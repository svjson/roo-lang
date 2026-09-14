# roo

`roo` is the command-line executable for running Roo files and packages.

It constructs a Roo runtime, applies command-line load paths, uses
`roo-package` when a `package.edn` is available, loads package native
libraries, and evaluates either a target file or a package's configured entry
point namespaces. From inside a package, it can also invoke a dependency's
declared `run` tool.

Basic usage:

```sh
roo file.roo
roo .
roo proof
roo --load-path src file.roo
roo -R vendor/packages proof
```

Arguments after the file or package target are forwarded to that target. Global
`roo` options such as `--help`, `--version`, `--load-path`, and
`--package-repository` must appear before the target.

Use `-R <dir>` or `--package-repository <dir>` to search an additional package
repository before sibling packages and the user-local repository. The option is
repeatable.

When a package `:main` returns an integer from `0` through `255`, `roo` uses it
as the process exit code. Other return values mean success. Generated package
executables use the same result contract.

`roo proof` is generic package tool dispatch: the current package must depend
on a package named `proof`, and that package must declare a `run` tool in its
`package.edn`. Arguments after the tool target are forwarded to that tool:

```sh
roo proof --reporter tree --filter '*checkout*'
```
