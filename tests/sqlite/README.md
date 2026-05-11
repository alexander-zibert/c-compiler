# SQLite integration tests

Drives `vendor/sqlite/bin.json` (SQLite 3.53.1 amalgamation) through `host.js`
to verify end-to-end SQL behavior. Each subdirectory is one test.

## Layout

```
tests/sqlite/<test_name>/
  test.sql           Required. SQL fed to the wasm sqlite shell on stdin.
  expected.txt       Required. Expected stdout, byte-exact.
  config.json        Optional. Per-test overrides (see below).
```

The runner builds `vendor/sqlite/bin.json` once, then for every subdirectory
under `tests/sqlite/` runs:

```
node host.js sqlite.wasm <shellArgs...>  < test.sql
```

and diffs stdout against `expected.txt`. Default `shellArgs` is `["-batch"]`,
which suppresses the banner and the interactive `sqlite>` prompts so output
is line-stable.

## config.json (optional)

All fields optional. Example:

```json
{
  "shellArgs": ["-batch", "-bail"],
  "prepare": ["PRAGMA foreign_keys = ON;"]
}
```

| Field | Default | Purpose |
|---|---|---|
| `shellArgs` | `["-batch"]` | Argv passed to the shell. Replaces, doesn't append. |
| `prepare` | `[]` | SQL statements prepended to `test.sql` (output discarded if `prepareDiscardOutput: true`, else included in expected). |
| `prepareDiscardOutput` | `false` | If true, run `prepare` in a separate shell invocation that writes to the same db file as `test.sql`. Requires `dbFile`. |
| `dbFile` | (in-memory) | Path (relative to test dir) for a persistent db. The runner uses a fresh temp copy per run. |
| `skip` | `false` | Skip this test. |

Keep test scripts deterministic — no `strftime('%s','now')`, no `random()` (or
seed it with `SELECT random()` only if the output isn't checked).

## Adding a test

1. Make `tests/sqlite/<name>/`
2. Write `test.sql`
3. `node host.js /tmp/sqlite.wasm -batch < test.sql > expected.txt` to capture
4. Run `python3 tests/run.py --types=sqlite -v` to verify
