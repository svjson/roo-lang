# proofread

`proofread` is a syntax and style checker for Roo source and EDN data.

It is intentionally small for now. The first version checks that every requested
file can be parsed by the Roo reader.

## Running proofread From Source

From the repository root:

```sh
./build/roo pkg/proofread <file|pattern>...
```

Examples:

```sh
./build/roo pkg/proofread path/to/file.roo
./build/roo pkg/proofread 'src/*.roo' package.edn
```

## Building the proofread Binary

From the repository root:

```sh
make build-proofread
```

This builds proofread with `rooc` and writes the executable under:

```text
build/proofread-install/build/proofread
```

## Installing the proofread Binary

From the repository root:

```sh
make install-proofread
```

This builds proofread with `rooc` and copies the executable to `$(PREFIX)/bin`.

The default `PREFIX` is:

- `~/.local` when `~/.local/bin` is already on `PATH`
- `/usr/local` otherwise

After installation:

```sh
proofread path/to/file.roo
```

## Commands

### check files

Check one or more files:

```sh
proofread path/to/file.roo package.edn
```

Check files matched by a shell-style pattern:

```sh
proofread 'src/*.roo'
```

If every file parses, proofread prints a short success summary:

```text
proofread: 2 files ok
```

If any file cannot be read, does not match, or fails to parse, proofread prints
the errors and exits with a non-zero status.

### help

Show usage:

```sh
proofread --help
```

### version

Show the proofread version:

```sh
proofread --version
```

## Current Limitations

- Style checks are not implemented yet.
- Recursive `**` glob behavior is not implemented yet.
- Parse diagnostics currently use the Roo reader's existing error messages.

## Tests

From the repository root:

```sh
ctest --test-dir build -R proofread --output-on-failure
```

This runs proofread's CLI smoke tests for valid Roo, valid EDN, and invalid
syntax.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
