# lisple

`lisple` is the command-line executable for running Lisple files and packages.

It constructs a Lisple runtime, applies command-line load paths, uses
`lisple-package` when a `package.edn` is available, loads package native
libraries, and evaluates either a target file or a package's configured entry
point namespaces. From inside a package, it can also invoke a dependency's
declared `run` tool.

Basic usage:

```sh
lisple file.lisple
lisple .
lisple proof
lisple --load-path src file.lisple
```

`lisple proof` is generic package tool dispatch: the current package must depend
on a package named `proof`, and that package must declare a `run` tool in its
`package.edn`.
