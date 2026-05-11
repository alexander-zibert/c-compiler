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
(buildroot with toolchain, more disk space) you'd need to point the
cfg at a different `root-riscv32.bin` and rebuild.

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
