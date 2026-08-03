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

The default `simple` reporter prints style warnings as files are checked. When
the run is complete, it prints a short summary:

```text
----------------------------------------
proofread: 2 files, 0 warnings, 0 errors
----------------------------------------
```

Style warnings include the source location, severity, rule name, and message:

```text
src/app.roo:12:3: warning redundant-do: Redundant do form
```

Warnings are counted in the summary but do not make the command fail.

If a file cannot be read, does not match, or fails to parse, `proofread` prints
the error, includes it in the summary, and exits with a non-zero status.

Select the reporter explicitly with `--reporter`:

```sh
proofread --reporter simple src
```

Group warnings with `--group-by`. Criteria are applied in order, so
`rule,namespace` groups first by rule and then by namespace:

```sh
proofread --group-by rule,namespace src
```

Supported grouping criteria:

- `rule`
- `namespace`
- `file`
- `package`

Repeat `--group-by` to build the same cascade incrementally:

```sh
proofread --group-by package --group-by rule src
```

## Style Warnings

Supported style warnings:

- `redundant-do`: reports unnecessary `do` forms, including single-expression
  wrappers and `do` forms that wrap a whole body where multiple expressions are
  already allowed.
- `redundant-function`: reports functions that only name a simple specialized
  call and are clearer when inlined.
- `use-when`: reports `if` forms with no meaningful else branch.
- `use-unless`: reports `if` forms with an empty then branch.
- `use-if-let`: reports `let` plus `if` forms that can be written as `if-let`.
- `use-when-let`: reports `let` plus `when` forms that can be written as
  `when-let`.
- `use-case`: reports `if` forms that dispatch on equality against a literal
  and `if` chains that dispatch on equality against literals for the same
  expression, which can be written as `case`.
- `use-cond`: reports nested predicate `if` chains that can be written as
  `cond`.
- `use-head`: reports `(nth value 0)` forms that can be written as `head`.
- `use-empty?`: reports count comparisons to zero that can be written as
  `empty?`.
- `use-not-empty?`: reports positive count checks and negated `empty?` calls
  that can be written as `not-empty?`.
- `use-not=`: reports negated equality checks that can be written as `not=`.
- `use-nil?`: reports equality checks against `nil` that can be written as
  `nil?`.
- `use-some?`: reports negated `nil?` checks that can be written as `some?`.
- `use-any?`: reports `find-index` and `find-first` calls used only as
  presence checks, which can usually be written as `any?`.
- `use-contains?`: reports `index-of` calls used only as presence checks,
  which can usually be written as `contains?`.
- `redundant-some?`: reports `some?` checks around non-predicate keyword
  lookups in predicate positions.
- `redundant-empty-fallback`: reports nil-tolerant sequence calls that
  defensively wrap input in `(or value [])`.
- `use-get-with-default`: reports `(or (get value key) default)` forms that
  can be written as `(get value key default)`.
- `use-keyword-lookup`: reports `(get value :key)` forms that can be written as
  `(:key value)`.
- `kebab-case-name`: reports symbol identifiers containing underscores.
- `redundant-map-check`: reports redundant `map?` guards before keyword lookup
  comparisons.
- `use-flat-map`: reports `(flatten (map seq f))` forms that can be
  written as `flat-map`.
- `use-keep`: reports `map` forms with inline functions whose single body form
  is `when` or `unless` and can be written as `keep`.
- `use-select-keys`: reports map literals that copy three or more matching keys
  from the same source map and can be written as `select-keys`.

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
