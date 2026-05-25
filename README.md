# Lisple

A modern, Clojure-inspired scripting language implemented in C++20, built to embed in C++ host applications.

Lisple exposes a compact s-expression language with first-class functions, lexical closures,
pattern destructuring, a namespace system, and direct integration with C++ host objects. It
compiles to a typed IR for evaluation rather than interpreting the AST directly.

## Features

- S-expression syntax with vectors `[...]`, maps `{...}`, and keyword access `(:key map)`
- First-class functions and lexical closures
- Pattern destructuring in function arguments
- Named and optional/rest parameters
- Namespace system with import and aliasing
- Powerful C++ interop macros for exposing native host objects to scripts
- Embeds as a static or shared library with a single `#include`

## Repository Overview

- bin/
  - **[lisple](bin/lisple/README.md)** - Command-line executable for running Lisple files and packages.
  - **[lisplec](bin/lisplec/)** - Compiler executable for generating bundled Lisple application builds.
- lib/
  - **[liblisple](lib/liblisple/README.md)** - Core embeddable Lisple runtime.
  - **[lisple-server](lib/lisple-server/README.md)** Small server layer for remote Lisple message parsing and dispatch experiments.
  - **[lib/lisple-package](lib/lisple-package/README.md)** Package manifest, dependency load paths, and native package loading.
- pkg/
  - **[loom](pkg/loom/README.md)** - Lisple package manager.
  - **[proof](pkg/proof/README.md)** - Lisple test framework package.

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

Sequence operations compose naturally with the thread-last macro `->>`:

```lisp
(def vehicles [{:name "Saloon" :seats 4}
               {:name "Compact" :seats 2}
               {:name "Van" :seats 8}])

(->> vehicles
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
    :default  :large))
```

## Embedding

Add `liblisple.a` or `liblisple.so` to your build and include `<lisple/runtime.h>`.

### Basic evaluation

```cpp
#include <lisple/runtime.h>

Lisple::Runtime rt;

auto result = rt.eval("(+ 1 2)");
result->i64();   // 3

rt.eval("(defun square [x] (* x x))");
rt.eval("(square 9)")->i64();   // 81
```

### Providing a host namespace

```cpp
#include <lisple/runtime.h>
#include <lisple/namespace.h>

Lisple::Namespace app_ns("app");
app_ns.def("pi", Lisple::Value::number(3.14159265));
app_ns.def("version", Lisple::Value::string("1.0"));

Lisple::Runtime rt(app_ns);
rt.eval("(* app/pi 2)")->f64();   // 6.28318...
```

### Exposing C++ objects

Lisple provides a macro-based DSL for wrapping C++ types as first-class Lisple
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
#include <lisple/host/object.h>

inline Lisple::HostTypeRef PRODUCT_TYPE("product", "shop/make-product");

NATIVE_ADAPTER(ProductAdapter, Product, (name, price), (price));
```

**3. Accessor implementation** (`.cpp`) - `METHOD(x)` resolves to `get_x()` /
`set_x()` on the wrapped object:

```cpp
#include <lisple/host/accessor.h>

NATIVE_ADAPTER_IMPL(ProductAdapter, Product, &PRODUCT_TYPE, (name), (price));

ADAPTER_PROP_GET(ProductAdapter, METHOD(name));
ADAPTER_PROP_GET_SET(ProductAdapter, METHOD(price));
```

Properties that are themselves wrapped objects use the `ADAPTER(...)` qualifier:

```cpp
ADAPTER_PROP_GET(OrderAdapter, METHOD(product), ADAPTER(ProductAdapter));
```

**4. Usage from Lisple** - once registered in a namespace, the object behaves
like any map:

```lisp
(def p (shop/make-product {:name "Widget" :price 9.99}))

(:name p)               ;; => "Widget"
(:price p)              ;; => 9.99
(assoc! p :price 12.50) ;; mutate in place
(assoc  p :price 12.50) ;; return modified copy
```

See [object.h](lib/liblisple/include/lisple/host/object.h),
[accessor.h](lib/liblisple/include/lisple/host/accessor.h), and the
[host test examples](lib/liblisple/test/tests/host/) for namespace registration
and make-functions.

## Building

Requires CMake 3.10+ and a C++20-capable compiler.

```bash
make configure                   # Release build
make configure BUILD_TYPE=Debug  # Debug build
make build                       # Produces liblisple.a and liblisple.so
make install                     # Install to ~/.local (override with PREFIX=...)
```

### Tests

```bash
make configure BUILD_TYPE=Debug
make build
cd build && ctest                              # All tests
cd build/lib/liblisple/test && ./testlisple --gtest_filter="*map*"  # Filtered
```

## CMake integration

After `make install`, the package is available via `find_package`:

```cmake
find_package(lisple REQUIRED)
target_link_libraries(my_app PRIVATE Lisple::lisple)
```

## License

© 2025 Sven Johansson. [MIT Licensed](./LICENSE)
