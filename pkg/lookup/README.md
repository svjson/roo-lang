# lookup

`lookup` builds Roo symbol index artifacts for editor tooling,
documentation lookup, and future language-server integrations.

The package is intentionally a skeleton for now. Its first real implementation
should produce a compact Roo-native index that can later be exported to
interoperability formats such as SCIP.

## Intended Index Scope

The index should describe:

- Roo built-in symbols.
- Symbols defined by Roo source files in a package or source tree.
- Symbols provided by installed package dependencies.
- Native symbols exposed by packages through metadata.

The generated artifact should not be embedded into `package.edn`. Package
manifests may eventually point to an index artifact, or the package format may
define a conventional path for it.

## Running From Source

From the repository root:

```sh
./build/roo pkg/lookup index pkg/lookup
```

Ask for position-aware information about a buffer by passing the current
source on standard input:

```sh
printf '(ns sample.core)\n(let [command-value 1]\n  command-value)\n' \
  | ./build/roo pkg/lookup thing-at . src/sample.roo 3 3
```

Build a standalone `lookup` command with:

```sh
./build/rooc build pkg/lookup --build-dir build/lookup-install --name lookup
```

Then run:

```sh
./build/lookup-install/build/lookup --help
```

## Current Status

`lookup` currently provides a symbol index and a position-aware `thing-at`
query for editor tooling.  The native reader bridge is:

```roo
(lookup.native/read-file-forms! path)
```

The bridge reads one Roo source file and returns top-level forms as ordinary
Roo data with source location metadata.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
