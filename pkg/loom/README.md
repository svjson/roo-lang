# Loom

Loom is a Roo package manager.

It manages packages in a shared local repository instead of creating project-local dependency trees. A project can depend on a package by name and version, and Roo tooling can resolve that package from the local repository.

Remote repositories are not implemented yet. For now, Loom installs packages from local source directories.

## Local Repository

The default repository is:

```text
~/.local/share/roo/pkg
```

Packages are stored by name and version:

```text
~/.local/share/roo/pkg/<name>/<version>
```

For example:

```text
~/.local/share/roo/pkg/proof/0.1.0/package.edn
```

Projects can then refer to installed packages by version:

```edn
{:dependencies {proof "0.1.0"}}
```

Use `--repo <dir>` with any Loom command to operate on a different repository root.

## Running Loom From Source

From the repository root:

```sh
./build/roo pkg/loom <command>
```

Example:

```sh
./build/roo pkg/loom list
```

## Installing the Loom Binary

From the repository root:

```sh
make install-loom
```

This builds Loom with `rooc` and copies the executable to `$(PREFIX)/bin`.

The default `PREFIX` is:

- `~/.local` when `~/.local/bin` is already on `PATH`
- `/usr/local` otherwise

After installation:

```sh
loom list
```

## Commands

### install

Copy a local package into the repository:

```sh
loom install /path/to/package
```

This reads `/path/to/package/package.edn`, validates the manifest, and installs the package to:

```text
<repo>/<name>/<version>
```

If that package version already exists, Loom removes the old installed version first. This prevents stale files from surviving reinstalls.

### link

Install a package as a symlink to its source directory:

```sh
loom link /path/to/package
```

This creates:

```text
<repo>/<name>/<version> -> /path/to/package
```

Linked packages are useful during local development. Other projects can depend on them by name and version without knowing where the source checkout lives:

```edn
{:dependencies {pixils "0.1.0"}}
```

`install --link` is equivalent:

```sh
loom install /path/to/package --link
```

### uninstall

Remove an installed package version:

```sh
loom uninstall proof@0.1.0
```

The two-argument form is also accepted:

```sh
loom uninstall proof 0.1.0
```

For linked packages, uninstall removes the symlink from the local repository. It does not remove the source package directory.

### list

List installed package versions:

```sh
loom list
```

Output:

```text
loom@0.1.0
proof@0.1.0
```

### info

Show manifest information for an installed package:

```sh
loom info proof@0.1.0
```

Output includes the installed path, description, load roots, dependencies, and native libraries.

### deps

Display the dependency graph for a package directory:

```sh
loom deps /path/to/package
```

Tree output uses the same line-drawing style as `tree`:

```text
app@0.1.0
├── pixils@0.1.0
│   └── proof@0.1.0
└── proof@0.1.0
```

Missing packages are marked:

```text
app@0.1.0
└── missing@1.0.0 (! missing)
```

Repeated dependency markers are opt-in:

```sh
loom deps /path/to/package --show-repeated
```

Flat output is also available:

```sh
loom deps /path/to/package --flat
```

## Package Manifests

Loom expects each package directory to contain `package.edn`.

Minimal example:

```edn
{:name my-package
 :version "0.1.0"
 :dependencies []
 :load-roots ["src"]}
```

Versioned dependencies resolve through the local repository:

```edn
{:dependencies {proof "0.1.0"}}
```

Local path dependencies are also understood by `loom deps`:

```edn
{:dependencies {proof "file:../proof"
                pixils {:path "../pixils"}}}
```

Path dependencies are resolved relative to the package that declares them.

## Current Limitations

- There is no remote registry or fetch command yet.
- There is no lockfile yet.
- `info` does not yet display whether an installed package is copied or linked.
- Version conflict reporting is still basic.

## Tests

From the repository root:

```sh
make test:loom-cli
```

This runs Loom's proof suite and CLI smoke tests.

## License

© 2026 Sven Johansson. [MIT Licensed](../../LICENSE)
