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

Build a standalone `lookup` command with:

```sh
./build/rooc build pkg/lookup --build-dir build/lookup-install --name lookup
```

Then run:

```sh
./build/lookup-install/build/lookup --help
```

## Current Status

`lookup` currently provides the package and command skeleton plus the first
native reader bridge:

```roo
(lookup.native/read-file-forms! path)
```

The bridge reads one Roo source file and returns top-level forms as ordinary
Roo data with source location metadata. The symbol-index analyzer is not
implemented yet.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
