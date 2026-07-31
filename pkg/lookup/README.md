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

`index` defaults to the symbol extractor. Select specific extractors with
repeated `-x` options:

```sh
./build/roo pkg/lookup index -x symbols pkg/lookup
./build/roo pkg/lookup index -x forms pkg/lookup
./build/roo pkg/lookup index -x forms -x symbols pkg/lookup
```

Write the generated EDN to a file with `-o`:

```sh
./build/roo pkg/lookup index -x symbols -o build/lookup.edn pkg/lookup
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
  (greet \"Ada\")
  ```

  See Also:
  - `str`"
  [name]
  (str "Hello " name))
````

The first prose paragraph becomes `:summary`. Later prose paragraphs before a
section heading become `:body`. Inline backticks are preserved in summary and
body text so documentation renderers can treat them as Markdown.

Recognized section headings are:

- `Args:`
- `Returns:`
- `Examples:`
- `See Also:`
- `Since:`
- `Deprecated:`

Argument lines use bullets of the form `- name: description`. The argument
name may be wrapped in backticks in the docstring. Argument docs are copied
into both the symbol doc map and the matching function signature parameter.
`Returns:` records return documentation, fenced `Examples:` blocks are emitted
as Roo examples, and `See Also:` bullets are emitted as symbol names without
backticks.

For now, `:doc-source` points at the source range of the defining form. The
source reader does not yet expose separate source metadata for child string
literals.

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

`lookup` currently provides a source-form index, a symbol index, selectable
index extraction, file output, and a position-aware `thing-at` query for editor
tooling. The source reader bridge is:

```roo
(lookup.reader/read-file-forms! path)
```

The bridge reads one Roo source file and returns top-level forms as ordinary
Roo data with source location metadata.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
