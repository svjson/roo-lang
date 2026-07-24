# Roo

A modern, Clojure-inspired scripting language implemented in C++20, built to embed in C++ host applications.

Roo exposes a compact s-expression language with first-class functions, lexical closures,
destructuring bindings, a namespace system, and direct integration with C++ host objects.
It lowers forms to an executable IR for evaluation rather than interpreting the AST directly.

## Features

- S-expression syntax with vectors `[...]`, maps `{...}`, and keyword access `(:key map)`
- First-class functions and lexical closures
- Map and vector destructuring in bindings, with map destructuring in function arguments
- Required, optional, and rest parameters
- Namespace system with import and aliasing
- Powerful C++ interop macros for exposing native host objects to scripts
- Embeds as a static or shared C++ library

## Repository Overview

- bin/
  - **[roo](bin/roo/README.md)** - Command-line executable for running Roo files and packages.
  - **[rooc](bin/rooc/)** - Compiler executable for generating bundled Roo application builds.
- lib/
  - **[libroo](lib/libroo/README.md)** - Core embeddable Roo runtime.
  - **[roo-server](lib/libroo-server/README.md)** - Small server layer for remote Roo message parsing and dispatch experiments.
  - **[roo-package](lib/libroo-package/README.md)** Package manifest, dependency load paths, and native package loading.
- pkg/
  - **[lookup](pkg/lookup/README.md)** - Roo indexing for tool-support.
  - **[loom](pkg/loom/README.md)** - Roo package manager.
  - **[proof](pkg/proof/README.md)** - Roo test framework package.
  - **[proofread](pkg/proofread/README.md)** - Roo syntax checker.
  - **[footsteps](pkg/footsteps/README.md)** - Stepwise work/progress orchestration.
  - **[workbook](pkg/workbook/)** - Persistent structured workbook values.
  - **[spool](pkg/spool/)** - Tree-shaped progress/reporting helpers.
  - **[i18n](pkg/i18n/)** - Small localization helper package.

## Language

Functions support required, optional, and rest parameters:

```lisp
(defun greet [name & title]
  (if title
    (str title " " name)
    name))

(greet "Smith")          ;; => "Smith"
(greet "Smith" "Dr.")    ;; => "Dr. Smith"
```

Map arguments can be destructured directly in the parameter list:

```lisp
(defun summarise [{:keys [name seats]}]
  (str name " (" seats " seats)"))

(summarise {:name "Vroom Deluxe" :seats 2})  ;; => "Vroom Deluxe (2 seats)"
```

Functions are first-class and close over their environment:

```lisp
(defun make-adder [n]
  (fn [x] (+ x n)))

(def add-ten (make-adder 10))
(add-ten 5)    ;; => 15
```

Sequence operations compose naturally with the thread-first macro `->`:

```lisp
(def vehicles [{:name "Saloon" :seats 4}
               {:name "Compact" :seats 2}
               {:name "Van" :seats 8}])

(-> vehicles
    (filter (fn [v] (> (:seats v) 3)))
    (map    (fn [v] (:name v))))
;; => ["Saloon" "Van"]
```

Branching via `cond`, `case`, `if`, and `when`:

```lisp
(defun describe-size [n]
  (cond
    (< n 10)  :small
    (< n 100) :medium
    :else     :large))
```

## Embedding

Link `libroo` into your build and include `<roo/runtime.h>` for the core runtime.
Host object adapters use the additional headers shown below.

### Basic evaluation

```cpp
#include <roo/runtime.h>

Roo::Runtime rt;

auto result = rt.eval("(+ 1 2)");
result->i64();   // 3

rt.eval("(defun square [x] (* x x))");
rt.eval("(square 9)")->i64();   // 81
```

### Providing a host namespace

```cpp
#include <roo/runtime.h>
#include <roo/namespace.h>

Roo::Namespace app_ns("app");
app_ns.store("pi", Roo::Value::number(3.14159265));
app_ns.store("version", Roo::Value::string("1.0"));

Roo::Runtime rt(app_ns);
rt.eval("(* app/pi 2)")->f64();   // 6.28318...
```

### Exposing C++ objects

Roo provides a macro-based DSL for wrapping C++ types as first-class Roo
values. Wrapped objects are accessible via keyword accessors and participate in
`assoc` / `assoc!` just like plain maps.

**1. The C++ type** - no changes required to the existing class:

```cpp
class Product {
  std::string _name;
  double _price;
public:
  const std::string& get_name() const;
  double get_price() const;
  void set_price(double price);
};
```

**2. Adapter declaration** (header) - `NATIVE_ADAPTER` lists readable and
writable properties:

```cpp
#include <roo/host/object.h>

inline Roo::HostTypeRef PRODUCT_TYPE("product", "shop/make-product");

NATIVE_ADAPTER(ProductAdapter, Product, (name, price), (price));
```

**3. Accessor implementation** (`.cpp`) - `METHOD(x)` resolves to `get_x()` /
`set_x()` on the wrapped object:

```cpp
#include <roo/host/accessor.h>

NATIVE_ADAPTER_IMPL(ProductAdapter, Product, &PRODUCT_TYPE, (name), (price));

NOBJ_PROP_GET__METHOD(ProductAdapter, name);
NOBJ_PROP_GET_SET__METHOD(ProductAdapter, price);
```

Properties that are themselves wrapped objects use the `ADAPTER(...)` qualifier:

```cpp
ADAPTER_PROP_GET(OrderAdapter, METHOD(product), ADAPTER(ProductAdapter));
```

**4. Usage from Roo** - once registered in a namespace, the object behaves
like any map:

```lisp
(def p (shop/make-product {:name "Widget" :price 9.99}))

(:name p)               ;; => "Widget"
(:price p)              ;; => 9.99
(assoc! p :price 12.50) ;; mutate in place
(assoc  p :price 12.50) ;; return modified copy
```

See [object.h](lib/libroo/include/roo/host/object.h),
[accessor.h](lib/libroo/include/roo/host/accessor.h), and the
[host test examples](lib/libroo/test/tests/host/) for namespace registration
and make-functions.

## Building

Requires CMake 3.20+ and a C++20-capable compiler.

```bash
make configure                   # Release build
make configure BUILD_TYPE=Debug  # Debug build
make build                       # Builds libraries, tools, and local development package links
make install                     # Install to ~/.local (override with PREFIX=...)
make release                     # Build a staged release archive under dist/
```

## Installing A Release

Download the archive for your platform from the GitHub Release page, unpack it,
and add its `bin` directory to `PATH`.

```bash
tar -xzf roo-0.1.0-linux-x86_64.tar.gz
export PATH="$PWD/roo-0.1.0-linux-x86_64/bin:$PATH"
roo --version
```

Release archives include `roo`, `rooc`, libraries, headers, CMake package files,
and bundled Roo packages under `share/roo/pkg`.

See [Installing Roo](INSTALL.md) for checksum verification, Windows usage,
bundled packages, and CMake integration.

### Tests

```bash
make configure BUILD_TYPE=Debug
make build
ctest --test-dir build --output-on-failure     # All CTest tests
cd build/lib/libroo/test && ./testroo --gtest_filter="*map*"  # Filtered
```

## CMake integration

After `make install`, the package is available via `find_package`:

```cmake
find_package(roo REQUIRED)
target_link_libraries(my_app PRIVATE Roo::roo)
```

## License

© 2025 Sven Johansson. [MIT Licensed](./LICENSE)
