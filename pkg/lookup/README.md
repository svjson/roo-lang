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

Current canonical index outputs are:

- `:roo/source-forms`
- `:roo/symbol-index`
- `:roo/index` (combined forms + symbols)
- `:roo/lookup-audit` (audit diagnostics)

## Running From Source

From an installed `lookup` CLI:

```sh
lookup index ./path/to/package
```

`index` defaults to the symbol extractor when indexing a package. Select specific
extractors with repeated `-x` options:

```sh
lookup index -x symbols ./path/to/package
lookup index -x forms ./path/to/package
lookup index -x forms -x symbols ./path/to/package
```

Index generic source roots with `--root`. A root-only index defaults to the
native extractor:

```sh
lookup index --root ./path/to/native/include --root ./path/to/native/src
```

Skip paths or subtrees under indexed roots with repeated `--exclude` options:

```sh
lookup index --root ./path/to/native --exclude ./path/to/native/test
```

Root-based indexes can carry explicit package metadata. This is useful when
indexing sources that belong to a package but do not live under a Roo package
root:

```sh
lookup index \
  --root ./path/to/native/include \
  --root ./path/to/native/src \
  --package-name my-package \
  --package-version 1.2.3
```

Use `--package-description` to include a package description. `--package-version`
and `--package-description` require `--package-name`.

Write the generated EDN to a file with `-o` or `--out`:

```sh
lookup index -x symbols -o build/lookup.edn ./path/to/package
lookup index --root ./path/to/native/src -x native -o build/native.edn
```

## Roo Docstrings

`lookup` reads Roo docstrings from `def` and `defun` forms. A docstring is a
string literal inside the defining form:

````roo
(def answer
  "The answer used by examples.

  Since:
  0.1.0"
  42)

(defun greet
  "Build a greeting for `name`.

  Uses a friendly default format.

  Args:
  - `name`: Person to greet.

  Returns:
  A greeting string.

  Examples:
  ```roo
  (greet "Ada")
  ```

  See Also:
  - `str`"
  [name]
  (str "Hello " name))
````

The output format is currently captured as `:markdown`, with section-aware parsing
for Args, Returns, Examples, See Also, Since, and Deprecated.

`lookup` does not yet extract namespace (`ns`) docstrings in the current
implementation. That backlog item is tracked in
`docs/lookup/backlog.md`.

## Current Status

`lookup` currently provides a source-form index, a symbol index, selectable index
extraction, file output, and a position-aware `thing-at` query for editor tooling.
The source reader bridge is:

```roo
(lookup.reader/read-file-forms! path)
```

The bridge reads one Roo source file and returns top-level forms as ordinary Roo
data with source location metadata.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
