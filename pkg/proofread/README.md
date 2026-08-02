# proofread

`proofread` checks Roo source and EDN files.

It verifies that each requested file can be read and parsed. Roo source files
may also produce style warnings.

## Basic Use

Check one or more files:

```sh
proofread src/app.roo package.edn
```

Check files matched by a shell-style pattern:

```sh
proofread 'src/*.roo'
```

Quote patterns when you want `proofread` to expand them instead of your shell.

## Output

When every requested file parses, `proofread` prints a short summary:

```text
proofread: 2 files ok
```

Style warnings include the source location, severity, rule name, and message:

```text
src/app.roo:12:3: warning redundant-do: Redundant do form with a single expression
```

Warnings are informational and do not make the command fail.

If a file cannot be read, does not match, or fails to parse, `proofread` prints
the error and exits with a non-zero status.

## Style Warnings

Implemented style warnings:

- `redundant-do`: reports `(do expr)` forms that wrap a single expression.

## Commands

Show help:

```sh
proofread --help
```

Show the installed version:

```sh
proofread --version
```

## Running From Source

From a Roo source checkout:

```sh
./build/roo pkg/proofread src/app.roo
```

## Installing

From a Roo source checkout:

```sh
make install-proofread
```

This installs the `proofread` command under the configured installation prefix.

## Current Limitations

- Style warnings do not affect the exit status.
- Style check configuration is not implemented yet.
- Recursive `**` glob behavior is not implemented yet.
- Parse diagnostics currently use the Roo reader's existing error messages.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
