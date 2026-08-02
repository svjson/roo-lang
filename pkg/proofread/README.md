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

Check all Roo and EDN files under a directory:

```sh
proofread src
```

Quote patterns when you want `proofread` to expand them instead of your shell.

## Output

When every requested file parses, `proofread` prints a short summary:

```text
proofread: 2 files ok
```

Style warnings include the source location, severity, rule name, and message:

```text
src/app.roo:12:3: warning redundant-do: Redundant do form
```

Warnings are informational and do not make the command fail.

If a file cannot be read, does not match, or fails to parse, `proofread` prints
the error and exits with a non-zero status.

## Style Warnings

Implemented style warnings:

- `redundant-do`: reports unnecessary `do` forms, including single-expression
  wrappers and `do` forms that wrap a whole body where multiple expressions are
  already allowed.
- `use-when`: reports `if` forms with no meaningful else branch.
- `use-unless`: reports `if` forms with an empty then branch.
- `use-if-let`: reports `let` plus `if` forms that can be written as `if-let`.
- `use-when-let`: reports `let` plus `when` forms that can be written as
  `when-let`.

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
- Recursive `**` glob behavior is not implemented yet. Pass a directory to
  check a tree recursively.
- Parse diagnostics currently use the Roo reader's existing error messages.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
