# C Compiler

## Portability

`compiler.js` MUST work in both browser and Node.js environments. Never use `process.env`, `process.stderr`, `process.exit`, `process.hrtime`, or any other Node.js-specific API without a `typeof process !== 'undefined'` guard and a browser-compatible fallback. No environment variables — use compiler options and CLI flags instead.

## TODOs

Planned work and design docs live in the `todos/` folder. Each file covers a distinct feature or topic. Check there before starting new work to see what's already been planned.
