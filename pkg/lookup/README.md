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

## Incremental Index State

Maintain a dynamic symbol index as a manifest and independently replaceable
source contributions with `--state`:

```sh
lookup index ./path/to/package --state .lookup/index
```

The first invocation builds the complete state. Later invocations without an
explicit update reconcile source modification times. For an editor save, name
the known-dirty source and request a machine-readable delta on stdout:

```sh
lookup index ./path/to/package \
  --state .lookup/index \
  --update ./path/to/package/src/foo.roo \
  --emit-delta edn
```

`--update` is repeatable. Roo sources are replaced independently. A changed
native source currently causes a complete state rebuild because native symbol
extraction may join declarations and documentation across files. Diagnostics go
to stderr through the normal CLI error path; stdout contains the EDN delta.

Use `-o` together with state mode when a complete compatibility snapshot is
also required. The snapshot is not otherwise reconstructed during a delta-only
editor update:

```sh
lookup index ./path/to/package \
  --state .lookup/index \
  --update ./path/to/package/src/foo.roo \
  --emit-delta edn \
  -o build/lookup.edn
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

## Native Implementation Documentation

Documentation for native-implementations of Roo symbols can be provided via doc
comments detailing the Roo API exposed by C++ code. Descriptions, parameters,
return values, and examples should use Roo names and calling conventions.

### Comments on `FUNC` and `SPECIAL_FORM_DECL`

Place the documentation block immediately before the declaration:

```cpp
/*!
 * @brief Read the complete contents of a text file as a string.
 * @since 0.1.0
 * @see roo.io/spit!
 *
 * Usage:
 * @code
 * (roo.io/slurp! "notes.txt")
 * => "file contents"
 * @endcode
 *
 * | Arg  | Description                   |
 * | ---- | ----------------------------- |
 * | path | Path of the file to read.     |
 *
 * @return The file contents.
 */
FUNC(SlurpBangFunction, slurp)
```

No symbol or kind annotations are needed here. `FUNC` identifies a function and
`SPECIAL_FORM_DECL` identifies a special form. Lookup joins the declaration to
its exported Roo name through the implementation identity:

```cpp
/** SlurpBangFunction - roo.io/slurp! */
FUNC_IMPL(SlurpBangFunction, ...)
```

The class name in the identity must match the class name in the documented
declaration.

### Comments on non-identifiable C++ symbol.

For a comment that is not attached to `FUNC` or `SPECIAL_FORM_DECL`, add
`@roo.symbol` and `@roo.kind`.

These can occur at any level and is not in any way tied to the C++ syntax
that follow it.

```cpp
/*!
 * @roo.symbol proof.syntax/given
 * @roo.kind special-form
 * @brief Seed the current state of a test scenario.
 * @since 0.1.0
 * @see proof.syntax/when
 * @see proof.syntax/then
 *
 * | Arg     | Description                 |
 * | ------- | --------------------------- |
 * | body... | State-producing body forms. |
 *
 * @return The final scenario state.
 */
ns->store("given", PhaseForm::make("given"));
```

`@roo.symbol` is the qualified Roo name.
`@roo.kind` accepts `function`, `special-form`, `macro`, `var`, or `constant`.

Put a separate documentation block on each exported Roo symbol when several
symbols share a native implementation.

### Supported fields

Lookup extracts the following content from either form:

- `@brief` supplies the summary. Separate prose paragraphs supply the body.
- `@code`/`@endcode` blocks supply examples. A format after `@code` overrides
  the default `:roo` example format.
- Markdown-style argument tables supply parameter names and documentation.
  Multiple argument tables describe multiple signatures.
- `@return` or `@returns` supplies return documentation.
- Repeated `@see` annotations supply related Roo symbols.
- `@since` records the version in which the Roo API appeared.
- `@deprecated` marks the Roo API as deprecated and may include replacement
  guidance on the same line.

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
