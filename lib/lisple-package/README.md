# lisple-package

`lisple-package` is the package loading layer that sits next to `liblisple`.
It reads `package.edn`, resolves local package dependencies, builds Lisple load
paths, loads native libraries declared by a package, and loads package bootstrap
namespaces.

This library is deliberately outside `liblisple`. `liblisple` stays an
embeddable language runtime; embedders that want package loading can opt into
this layer and wire it into their runtime.

This document describes how the package layer works so far. The shape is usable,
but it is not a final package manager or distribution format yet.

## Package Manifest

A package is a directory containing `package.edn`:

```lisp
{:name example-app
 :version "0.1.0"
 :description "Example package."
 :dependencies {utility "file:../utility"
                example-runner "file:../example-runner"}
 :load-roots ["src"]
 :autoloads [example.bootstrap]
 :config {example-runner {:mode :fast}}
 :tools {run example.tools/run}
 :entry-points [example.app]
 :run example-runner}
```

Current fields:

- `:name` is the package name.
- `:version` is the package version string.
- `:description` is optional descriptive text.
- `:dependencies` maps dependency names to local package locations or metadata.
- `:load-roots` lists package-relative Lisple source roots.
- `:namespace-roots` maps namespace prefixes to package-relative source roots.
- `:native-libraries` lists package-relative or absolute native libraries.
- `:autoloads` lists namespaces loaded after native libraries are available.
- `:config` maps package names to package-specific configuration data.
- `:tools` maps tool names to qualified functions exposed by this package.
- `:entry-points` lists namespaces loaded when running the package directory.
- `:main` names a qualified function to invoke as the package application entry.
- `:run` names a dependency package whose `run` tool should be used as the
  package application runner.

## Pure Lisple Dependencies

Dependencies can be resolved from local paths or from the local Lisple package
repository.

The shortest form uses a `file:` string:

```lisp
{:dependencies {utility "file:../utility"}}
```

The expanded local path form can include a version:

```lisp
{:dependencies {utility {:version "0.1.0"
                         :path "../utility"}}}
```

If both `:version` and `:path` are specified, the dependency manifest at that
path must have the requested version.

Dependency paths are resolved relative to the package that declares them. The
load plan is dependency-first, so dependency `:load-roots` are added before the
depending package's own `:load-roots`.

Version-only dependencies are resolved from package search roots:

```lisp
{:dependencies {utility "0.1.0"}}
```

The local repository search root defaults to:

```text
~/.local/share/lisple/pkg
```

The expected repository layout is:

```text
~/.local/share/lisple/pkg/
  utility/
    0.1.0/
      package.edn
      src/
```

For compatibility with source-tree development, the resolver also keeps looking
for sibling package roots such as `../utility/package.edn`.

## Namespace Roots

By default, Lisple file namespace loading maps dots to directories:

```text
mylib.stuff.core -> mylib/stuff/core.lisple
```

That works, but it can force package directory layouts to repeat namespace
prefixes. A package can declare namespace roots when a namespace prefix should
start at a specific directory:

```lisp
{:name mylib
 :version "0.1.0"
 :load-roots ["src"]
 :namespace-roots {mylib.stuff "src/lisple/main-stuff"}}
```

With that manifest, loading `mylib.stuff.core` first tries:

```text
src/lisple/main-stuff/core.lisple
```

before falling back to relative namespace inference and then the default
full-path lookup.

Namespace roots are prefix mappings. The longest matching prefix wins, so a
more specific mapping such as `mylib.stuff.ui` takes precedence over `mylib`.

## Native Libraries

A package can declare native libraries in `:native-libraries`:

```lisp
{:name example
 :version "0.1.0"
 :load-roots ["src"]
 :native-libraries [{:name "example-native"
                     :version "0.1.0"
                     :path "native"
                     :namespaces [example.runtime]}]}
```

Fields:

- `:name` is the native package name reported by the library ABI.
- `:version` is the native package version reported by the library ABI.
- `:path` is either a library file path or a directory containing the library.
- `:namespaces` documents the namespaces the library is expected to provide.

When `:path` is a directory, the loader looks for the platform library filename
for `:name` in that directory:

- Linux: `lib{name}.so`
- macOS: `lib{name}.dylib`
- Windows: `{name}.dll`

For example, `{:name "example-native" :path "native"}` resolves to
`native/libexample-native.so` on Linux.

## Autoloads

Some packages need pure Lisple namespaces to be loaded eagerly because those
namespaces define globals, register values, or bridge native namespaces into a
package-level API. Use `:autoloads` for that:

```lisp
{:name example
 :version "0.1.0"
 :load-roots ["src"]
 :native-libraries [{:name "example-native"
                     :path "native"
                     :namespaces [example.runtime]}]
 :autoloads [example.bootstrap]}
```

Autoload namespaces are loaded after native libraries are registered with the
runtime and before package entry points are loaded. That means an autoload
namespace may require namespaces provided by a native library.

## Package Layout

A package that combines Lisple source and a native library can use this layout:

```text
example/
  package.edn
  README.md
  src/
    example/
      app.lisple
  native/
    CMakeLists.txt
    include/
      example/
        native.h
    src/
      native.cpp
```

The manifest points Lisple at `src` and points native loading at `native`:

```lisp
{:name example
 :version "0.1.0"
 :description "Example package."
 :dependencies []
 :load-roots ["src"]
 :namespace-roots {example "src/example"}
 :native-libraries [{:name "example-native"
                     :version "0.1.0"
                     :path "native"
                     :namespaces [example.runtime]}]
 :entry-points [example.app]}
```

The native library name and version in `package.edn` must match the values
returned by the native ABI.

## Native ABI

A native library must export this C symbol:

```c++
extern "C" LISPLE_NATIVE_EXPORT const LispleNativePackageV1*
lisple_native_package_v1();
```

The returned `LispleNativePackageV1` describes the library:

```c++
static const LispleNativePackageV1 package{
  LISPLE_NATIVE_ABI_VERSION,
  sizeof(LispleNativePackageV1),
  "example-native",
  "0.1.0",
  LISPLE_NATIVE_CXX_ABI,
  load_native_package,
  unload_native_package,
  last_error,
};
```

The loader checks:

- ABI version
- struct size
- package name
- package version
- Lisple ABI token

The `load` callback receives a `LispleNativeHostV1`. Register namespaces through
the host:

```c++
int load_native_package(const LispleNativeHostV1* host)
{
  auto ns = std::make_unique<Lisple::Namespace>("example.runtime");
  ns->set_origin(Lisple::Namespace::Origin::native());
  ns->store("answer", AnswerFunction::make());
  return host->register_namespace(host->user, ns.release());
}
```

After `register_namespace` succeeds, ownership of the namespace has moved to the
runtime.

## Building a Native Library

Build the native component as a shared library. The shared library must link
against `liblisple` and `lisple-package`, because it uses Lisple runtime types
and the native package ABI.

A minimal CMake target looks like this:

```cmake
add_library(example_native SHARED
  src/native.cpp
)

set_target_properties(example_native PROPERTIES
  OUTPUT_NAME example-native
)

target_include_directories(example_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(example_native
  PUBLIC
    Lisple::lisple_shared
    Lisple::package_shared
)
```

If the manifest uses `:path "native"`, copy the built artifact into that
directory after building:

```cmake
add_custom_command(TARGET example_native POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/../native
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:example_native>
          ${CMAKE_CURRENT_SOURCE_DIR}/../native/$<TARGET_FILE_NAME:example_native>
)
```

That copy step is not magic or required by the loader. It is just one simple way
to make the built library land where `package.edn` says it will be.

## Loading From an Embedded Application

Applications that embed `liblisple` and construct `Lisple::Runtime` themselves
can use `lisple-package` directly.

Link the host application against both libraries:

```cmake
find_package(lisple-package REQUIRED)

target_link_libraries(my_app
  PRIVATE
    Lisple::lisple_shared
    Lisple::package_shared
)
```

Use the package headers alongside the normal Lisple runtime headers:

```c++
#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>
#include <lisple-package/manifest.h>
#include <lisple-package/native_loader.h>
```

The standard sequence is:

1. Create a filesystem that can read package manifests.
2. Resolve a load plan from the root package directory.
3. Merge the package load paths with any host-provided load paths.
4. Construct the runtime with a filesystem that uses those load paths.
5. Configure package namespace roots on the runtime.
6. Load native libraries into the runtime.
7. Load package autoload namespaces.
8. Keep the filesystem and loaded native package handles alive while the runtime
   is alive.
9. Read a file or load the package entry point namespaces.

Minimal example:

```c++
Lisple::DirRootFileSystem manifest_fs("/");
auto plan = Lisple::Package::resolve_load_plan(manifest_fs, "/path/to/package");

auto fs = Lisple::Package::make_load_path_file_system(
  plan,
  {"/host/app/lisple"});
Lisple::Runtime runtime(fs.get());

Lisple::Package::configure_runtime_namespace_roots(runtime, plan);
auto native_packages = Lisple::Package::load_native_libraries(runtime, plan);
Lisple::Package::load_autoloads(runtime, plan);
runtime.read_file("example/app.lisple");
```

Keep both the filesystem and `LoadedNativePackages` alive for as long as the
runtime may need them. `LoadedNativePackages` keeps dynamic library handles open.

If the application already has its own load paths, use `merge_load_paths` or
`make_load_path_file_system(plan, extra_load_paths)`. Extra load paths come
first, then package source roots are appended in dependency-first order.

If the application already has its own filesystem abstraction, it can still use
`resolve_load_plan` and `merge_load_paths`, then build its own `FileSystem`
implementation from the returned paths. `make_load_path_file_system` is only the
convenience bridge for the common directory-root case.

To run package entry point namespaces, load each namespace listed in
`plan.entry_points` through an `ns` form:

```c++
for (const auto& entry_point : plan.entry_points)
{
  runtime.eval("(ns host.package-entry (:require " + entry_point + "))",
               "<package-entry>");
}
```

Package tools are ordinary qualified functions declared by a package:

```lisp
{:name proof
 :tools {run proof.runner/run}}
```

An application can provide package-specific configuration with `:config`:

```lisp
{:name app
 :dependencies {proof "file:../proof"}
 :config {proof {:test-roots ["test" "integration"]}}}
```

An application can also declare that a dependency package's `run` tool is its
own application runner:

```lisp
{:name app
 :dependencies {proof "file:../proof"}
 :run proof}
```

The `lisple` binary uses this to run dependency-provided tools. `lisple proof`
from inside the application package loads the normal package environment, finds
the `proof` dependency, then invokes its `run` tool with a context map containing
the application package root, package name, load paths, tool name, and the
`proof` config block.

## Loading From the `lisple` Binary

The `lisple` binary uses this layer for package directories.

Running a package directory:

```sh
lisple .
```

finds `package.edn`, resolves dependencies, merges package load roots into the
runtime load path, loads declared native libraries, loads `:autoloads`, and
requires the namespaces listed in `:entry-points`.

Running a file inside a package:

```sh
lisple test/example/app_test.lisple
```

uses the nearest parent `package.edn` to resolve the same package load paths and
native libraries, then loads `:autoloads` before reading the file.

Running a dependency-provided package tool:

```sh
lisple proof
```

uses the nearest parent `package.edn`, resolves the package and dependencies,
loads native libraries and autoloads, then invokes the dependency package's
declared `run` tool with package context data.

## Current Limitations

- This is not a package manager. There is no registry, download step, lockfile,
  install cache, publishing workflow, or semver resolver.
- Dependencies can be local paths or versioned packages in the local repository.
- The `package.edn` shape is still expected to change.
- Native ABI versioning is minimal and currently C++-oriented.
- Native libraries must be built separately and placed where `:path` points.
- There is no automatic build step for native libraries.
- `:namespaces` is metadata for the manifest/load plan; it does not create
  namespaces by itself. The native library must register them when loaded.
- There is no mixed namespace merge story yet. A namespace is currently provided
  by Lisple source or by a native library.
- Native library unload behavior is intentionally simple. Keep loaded package
  handles alive while the runtime may call into registered native code.
- Package tool dispatch currently runs the dependency package's `run` tool only.

## Existing Examples

- `pkg/proof` is the main package with Lisple source and a native library.
- `lib/lisple-package/test/tests/native_test_package.cpp` is a minimal native
  library used by package loader tests.
- `lib/lisple-package/test/tests/manifest_test.cpp` contains focused examples
  for manifest parsing, dependency resolution, load path creation, and native
  library loading.
