# TinyEMU 2019-12-21

Unmodified upstream sources from https://bellard.org/tinyemu/tinyemu-2019-12-21.tar.gz

- **Version**: 2019-12-21 (latest available; upstream has not released since)
- **Author**: Fabrice Bellard
- **License**: MIT (see `MIT-LICENSE.txt`)
- **SHA256**: `be8351f2121819b3172fcedce5cb1826fa12c87da1b7ed98f269d3e802a05555`

## What this is

TinyEMU is Bellard's emulator suite — most of the code here builds an
emulator for one of several CPUs and connects it to a small set of
virtio devices. This vendored copy is wired up to build the **RISC-V
32-bit** target as a headless Node program that boots Linux to a
BusyBox shell.

Original tarball contents include:

- RISC-V emulator (RV128IMAFDQC, 32/64/128-bit, FP, compressed):
  `riscv_cpu*.c`, `riscv_machine.c`
- x86 emulator (KVM-based, but the source is portable):
  `x86_cpu*.c`, `x86_machine.c`
- VirtIO devices (console, network, block, input, 9P): `virtio.c`
- Graphical display (SDL): `sdl.c`, `simplefb.c`, `vga.c`
- Soft-float implementation: `softfp.c` (with `softfp_template*.h`)
- Misc utilities: `cutils.c`, `aes.c`, `sha256.c`, `json.c`
- Filesystem support (disk, network, 9P): `fs*.c`
- Slirp user-mode networking: `slirp/`
- A JavaScript demo runtime: `js/`, `jsemu.c`

The build picks a subset of these — the rest are left in place
unmodified.

## Build and run

```bash
node compiler.js -o /tmp/tinyemu.js vendor/tinyemu/bin.json
node /tmp/tinyemu.js
```

The first command produces a self-contained ~10 MB Node bundle (wasm
binary + disk images embedded as base64). The second boots Linux 4.15
RISC-V 32-bit and drops to a BusyBox shell prompt (`~ #`) in roughly a
second. Run it from a real terminal for interactive use — `host.js`
puts stdin in raw mode when the wasm exports `console_queue_char`, so
keystrokes flow byte-by-byte into the guest.

Optional args: `node /tmp/tinyemu.js <cfg-name> [ram-mb]`. Defaults are
`root-riscv32.cfg` and `128`.

## What works / what doesn't

Wired up:

- Console I/O — `console_write` → `process.stdout`,
  `process.stdin` → `console_queue_char`.
- VirtIO block device — backed by a regular fopen'd file (see
  `block_file.c` and the override in `tinyemu_main.c`).
- Async event loop — `emscripten_async_call` from `host.js` drives
  the VM step callback after `main` returns.

Not wired (host stubs return success or zero):

- Network (`fs_net_init`, `block_device_init_http` for HTTP backing).
- 9P remote filesystem.
- Framebuffer / SDL display / mouse / keyboard SDL paths.
- The cfg disables eth0 and graphical output.

The disk image ships with a minimal BusyBox userland — no C compiler
inside the guest. `/` is 2.6 MB used of 3.9 MB. For a richer guest
(buildroot with toolchain, more disk space) see "Getting tcc / a
richer guest" below.

## Disk image and isolation

`bin.json` embeds four files from `disk/` (bbl32.bin, kernel-riscv32.bin,
root-riscv32.bin, root-riscv32.cfg) into the bundle. At startup the
bundle decodes them into `os.tmpdir()/cjs-<pid>/` and `process.chdir`s
there. Guest writes (the cfg mounts `rw`) affect only that per-PID
temp directory — the vendor `disk/` sources are read-only at build
time.

The temp dir is not auto-cleaned; remove `cjs-*` directories under
`os.tmpdir()` if you want to clean up.

## Files added on top of the upstream tarball

The upstream tarball did not ship these — they're specific to this
port:

- `block_file.c` — file-backed virtio block device. Implements
  `block_device_init(filename, mode)` using stdio. Adapted from
  Bellard's `temu.c` (the standalone TinyEMU CLI driver).
- `tinyemu_main.c` — wasm entry point. Calls `vm_start` with the cfg
  filename; overrides `block_device_init_http` to route through
  `block_device_init` (deferring the start callback via
  `emscripten_async_call` so the caller's `tab_drive[0].block_dev =
  ...` assignment lands before `init_vm` walks it); exports the
  `__no_exit_runtime` sentinel so `host.js` doesn't `process.exit`
  after `main` returns.
- `bin.json` — build config (sources, defines, embedded data files).
- `disk/` — bbl32.bin, kernel-riscv32.bin, root-riscv32.bin, and the
  cfg, taken from Bellard's RISC-V demo image
  (`diskimage-linux-riscv-2018-09-23.tar.gz`).

`machine.c` has one local patch — the upstream gated the synchronous
`load_file` path behind `#ifndef EMSCRIPTEN`; with `-DEMSCRIPTEN` set
for the JS demo, it calls `abort()`. The patch removes the gate so the
file-backed path always works.

## Getting tcc / a richer guest

The shipped disk image (`disk/root-riscv32.bin`, 4 MB ext2) is the
minimal busybox userland Bellard distributes with the TinyEMU tarball.
It has no compiler. Bellard's larger demos at https://bellard.org/jslinux
(`buildroot-riscv64.cfg`, `fedora33-riscv.cfg`) DO ship a full
buildroot with tcc — but there's no offline tarball for them. The
cfgs reference URLs like:

```
fs0: { file: "https://vfsync.org/u/os/buildroot-riscv64" },
```

`vfsync.org` serves the rootfs over a **custom HTTP-9P protocol** that
TinyEMU's `fs_net.c` / `fs_wget.c` consume lazily — there is no
directory listing, no tarball download, no public file URL. To use
that image we'd need to implement HTTP fetching on the host side and
wire up `fs_net_init` / `fs_wget` (currently stubbed in `host.js`).
Roughly an afternoon of work; ties operation to vfsync.org staying up.

Alternative: build a buildroot ext2 disk image with tcc included and
swap it in. The TinyEMU `cfg` only needs the new `root-*.bin` path —
the existing virtio-blk + `block_device_init` plumbing handles any
size. Likely 50–200 MB image; would need separate hosting since the
repo doesn't want big binaries.

Neither path is wired up today. The current setup is a "Linux boots,
console works" demo, not a development environment.

## x86: why it doesn't work from this source

The `vendor/tinyemu/bin.json` build is RISC-V 32-bit only.
TinyEMU's source DOES contain `x86_machine.c` (PCI bus, PIT, PIC, RTC,
IDE, etc — 2569 lines, real working code) and the JSLinux demo shows
x86 running in a browser. But you cannot build a working x86 emulator
from these sources, because:

- **`x86_cpu.c` is a stub** (96 lines, every function exits or
  returns 0). It is the placeholder for a KVM frontend, not a
  software interpreter.
- Per the upstream readme: *"It is not really an emulator because it
  uses the Linux KVM API to run the x86 code at near native
  performance."* On a real x86 Linux host the build links to KVM
  ioctls. KVM is a hardware-virtualization API — it requires the host
  CPU to be x86 and **cannot be compiled to wasm under any
  circumstance**.
- The x86 emulator visible at https://bellard.org/jslinux is a
  separate codebase Bellard wrote in 2011 for the original JS/Linux
  demo. It IS a software interpreter and IT compiles to wasm
  (`x86emu-wasm.wasm` in the jslinux tarball, ~600 KB), but Bellard
  has never released its source. He distributes only the compiled
  binary.

So TinyEMU and JSLinux share configs, disk images, and devices, but
TinyEMU has never had an open-source x86 CPU emulator. The
`x86_machine.c` PC platform code is dead weight without a CPU.

To add x86 to this project we'd need a different emulator entirely.
Open-source options:

- [v86](https://github.com/copy/v86) — JS-native x86 software
  emulator, runs Linux/Windows/DOS in browsers. ~30 KLOC JavaScript;
  would need substantial work to integrate.
- [Bochs](https://bochs.sourceforge.io/) — mature C++ x86 emulator,
  much heavier (~150 KLOC), would test our C++ support (we don't
  have any).
- An old QEMU TCG backend (i386 only) — large and complex.
- Hand-roll an x86 interpreter — months of work for a useful subset.

None of these are currently planned.

## Architecture notes

- The cfg loader is async-by-design. Upstream uses `fs_wget` to fetch
  config + kernel + bbl over HTTP and a chain of `emscripten_async_call`s
  to schedule each next step. Our build keeps the chain but routes
  every load through synchronous `fopen`/`fread`, scheduling the next
  callback explicitly via `emscripten_async_call(cb, opaque, 0)`.
- The wasm exports `console_queue_char` and `vm_start` so the host can
  drive the VM after `main` returns. Both are reached via the
  `__export name = name;` directive in `tinyemu_main.c`.
- The `__no_exit_runtime` export tells `host.js` to skip the
  `exit()` / `__run_atexits()` call after `main` returns and instead
  await indefinitely so the async event loop keeps firing.
