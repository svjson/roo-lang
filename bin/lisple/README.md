# lisple

`lisple` is the command-line executable for running Lisple files and packages.

It constructs a Lisple runtime, applies command-line load paths, uses
`lisple-package` when a `package.edn` is available, loads package native
libraries, and evaluates either a target file or a package's configured entry
point namespaces.

Basic usage:

```sh
lisple file.lisple
lisple .
lisple --load-path src file.lisple
```
