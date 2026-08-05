# proof

`proof` is the canonical test framework for Roo.

It is intentionally small: tests are ordinary Roo code, assertions are built
around a single primitive form, and scenario-style tests are just structured
blocks layered on top of normal execution.

## Basic Use

Require `proof.core` without an alias when writing tests:

```lisp
(ns sample.tests
  (:require proof.core))

(deftest addition
  (is (= 4 (+ 2 2))))

(run)
```

`run` executes the registered tests, prints a report, and returns the result
maps.

Example output:

```text
  PASS addition
----------------------------------------
proof: 1 passed, 0 failed, 1 total (1ms)
----------------------------------------
```

Use `run-selected` to run only registered tests matching a selection map:

```lisp
(run-selected {:filter "*checkout*"})
(run-selected {:namespace "app.checkout"})
(run-selected {:namespace "app"
               :include-sub-namespaces? true
               :filter "*discount*"})
```

Discovery and namespace loading still happen before selection. The `:filter`
option supports `*` as a wildcard and matches both bare test names and qualified
names such as `app.checkout-test/applies-discount`. The `:namespace` option
matches an exact namespace by default. Add `:include-sub-namespaces? true` to
also include namespaces below that prefix.

## Assertions

Use `is` for the canonical assertion form:

```lisp
(is (= 4 (+ 2 2)))
(is (> hp 0))
(is (nil? value))
```

`should` and `assert` are readability aliases for `is`:

```lisp
(should (= :ready state))
(assert (connected? socket))
```

`is`, `should`, and `assert` record a failure and stop the current test body.

Use `expect` when the test should continue after a failed check:

```lisp
(deftest receipt
  (expect (= 66 subtotal))
  (expect (= 5 discount))
  (expect (= 61 total)))
```

This is useful when one test should report several related failures in one run.

## Tests

Register tests with `deftest`:

```lisp
(deftest total-due-applies-discount
  (let [subtotal 66
        discount 5]
    (is (= 61 (- subtotal discount)))))
```

The test name is a symbol. The body is ordinary Roo code.

Use `clear!` if a test file or REPL session needs to reset the currently
registered tests and failures before defining new tests:

```lisp
(clear!)
```

## Scenarios

Syntactic support for the **given/when/then** idiom is provided by
`proof.syntax`. Require this namespace aliased to avoid colliding with the
`when` language form.

```roo
(ns sample.scenario-tests
  (:require proof.core
            [proof.syntax :as s]))

(deftest moving-onto-enemy-triggers-attack
  (s/given
    {:world (world/fixture :small-arena)
     :player (unit/fixture :player {:at [2 2]})
     :enemy (unit/fixture :enemy {:at [3 2]})})

  (s/when [{:keys [world player]}]
    (input/press world :right)
    (game/tick world 3)
    (events/drain world))

  (s/then [events {:keys [player]}]
    (is (= (unit/state player) :attacking))
    (is (= [:attack] (event/types events)))))
```

The phases execute in the order they occur. Each phase returns the value of its
last expression and the phases feed those values forward:

- `given` takes no arguments and returns the test fixture or precondition.
- `when` takes an argument vector and receives the `given` result.
- `then` takes an argument vector and receives the `when` result. It may also
  receive the `given` result as a second argument.

The argument vectors use normal Roo function binding, including destructuring.
If a phase does not need a value, omit it from the argument vector:

```roo
(deftest simple-scenario
  (s/given
    4)

  (s/when [value]
    (+ value 1))

  (s/then [result]
    (is (= result 5))))
```

## Fixtures

Require `proof.fixture` with an alias when a test needs a generated value that
should be reused:

```roo
(ns sample.fixture-tests
  (:require proof.core
            [proof.fixture :as fixture]))
```

Use `deffixture` to declare reusable fixture providers:

```roo
(fixture/deffixture commonborn-bootstrap
  {:storage :persistent
   :path "backend/commonborn-bootstrap.edn"
   :version 1
   :paths [[:known-people]
           [:boot-known-people]]}
  (expensive-bootstrap))

(fixture/deffixture hot-dog-salesman-fixture
  {}
  {:job :hot-dog-salesman})

(fixture/deffixture my-fixture
  {}
  {:fixture :my-fixture})
```

Fixture declaration options:

- `:storage`, either `:cache` or `:persistent`. The default is `:cache`.
- `:path`, the persistent fixture document path.
- `:version`, a persistent fixture version. Changing it regenerates the fixture.
- `:refresh?`, when true, always regenerates a persistent fixture.
- `:paths`, nested paths to retain from a generated value.

Declared fixtures can be bound explicitly with `using-fixtures`:

```roo
(fixture/using-fixtures [[bootstrap commonborn-bootstrap]
                         [character hot-dog-salesman-fixture]]
  (is (= (:known-people bootstrap)
         (:boot-known-people bootstrap)))
  (is (= :hot-dog-salesman (:job character))))
```

`deftest` also accepts an optional fixture vector:

```roo
(deftest carries-family-through-social-memory
  [[bootstrap commonborn-bootstrap]
   my-fixture
   [character hot-dog-salesman-fixture]]
  (is (= (:known-people bootstrap)
         (:boot-known-people bootstrap)))
  (is (= :hot-dog-salesman (:job character))))
```

Each fixture spec may be:

```roo
fixture-name
[binding fixture-name]
[binding fixture-name opts]
```

Bare `fixture-name` is shorthand for `[fixture-name fixture-name]`. Use the
vector form when the fixture has a complex name or when the local binding should
use a shorter alias.

Anonymous fixture forms are also available when a fixture is only used locally:

```roo
(fixture/using-cache-fixture [data "bootstrap/commonborn"
                              (fn [] (expensive-bootstrap))]
  body...)

(fixture/using-persistent-fixture [world-map "world/default-map.edn"
                                   (fn [] (generate-world-map))
                                   {:version 1
                                    :refresh? false
                                    :paths [[:landmarks]
                                            [:paths]]}]
  body...)
```

Persistent fixtures are stored under `test/fixtures` by default. A package may
configure another root:

```edn
{:config {proof {:fixture-root "test/fixtures"}}}
```

The same setting can be supplied on the command line:

```text
roo proof --fixture-root test/fixtures
```

## Results

`run` returns a vector of result maps:

```lisp
[{:name addition :status :pass :elapsed-ms 0}
 {:name bad-math
  :status :fail
  :elapsed-ms 1
  :message "Expected 5, got 4."
  :failures [{:message "Expected 5, got 4."}]}]
```

Every result has:

- `:name`
- `:status`
- `:elapsed-ms`, the test body execution time in milliseconds

A passing result has `:status :pass`.

A failing result has:

- `:status :fail`
- `:message`, the first failure message
- `:failures`, all recorded failures for that test

If a test body throws an unexpected runtime error, proof records an error result
and continues with the remaining tests:

- `:status :error`
- `:message`, the runtime error text

## Reporting

The default reporter is `:simple`, which streams one line per test as it runs:

```text
  PASS addition
  FAIL bad-math
  ERROR division-error
```

Set `:durations? true` to include each test's elapsed time in the streamed
report:

```lisp
(run-selected {:durations? true})
```

```text
  PASS addition (0ms)
```

Package-driven runs can use the tree reporter:

```lisp
{:config {proof {:test-roots ["test"]
                 :reporter :tree}}}
```

The same setting can be supplied on the command line when running proof as a
package tool:

```sh
roo proof --reporter tree --durations
```

That groups streamed results by source file:

```text
test/app/checkout-test.roo
├── PASS - checkout-total
└── FAIL - checkout-discount
```

With durations enabled:

```text
test/app/checkout-test.roo
├── PASS - checkout-total (1ms)
└── FAIL - checkout-discount (0ms)
```

Reporter behavior is implemented in ordinary Roo namespaces:

- `proof.reporter` contains reporter selection and summary printing.
- `proof.reporter.common` contains shared result-detail, duration, and
  file-display helpers.
- `proof.reporter.simple` streams the default one-line-per-test report.
- `proof.reporter.tree` streams grouped output by test file.

`proof.reporter` exposes summary helpers:

- `status-count`
- `result-summary`
- `summary-text`
- `print!`

For example:

```lisp
(let [results (run-selected {:filter "*checkout*"})
      summary (proof.reporter/result-summary results)]
  (is (= 0 (:failed summary))))
```

## Package Tests

When proof is a dependency, the `roo` binary can launch proof through package
tool dispatch:

```lisp
{:name my-package
 :version "0.1.0"
 :dependencies {proof "file:../path/to/proof"}
 :load-roots ["src" "test"]}
```

Then run:

```sh
roo proof
```

By default, proof discovers and loads `.roo` files under the package's
`test/` directory, then runs the registered tests.

Test namespaces should require `proof.core` and define tests:

```lisp
(ns my-package.tests
  (:require proof.core
            my-package.core))

(clear!)

(deftest works
  (is (= 42 (my-package.core/answer))))
```

If tests live elsewhere, configure proof in `package.edn`:

```lisp
{:name my-package
 :dependencies {proof "file:../path/to/proof"}
 :load-roots ["src" "test" "integration"]
 :config {proof {:test-roots ["test" "integration"]}}}
```

The same config map can select tests for `roo proof`:

```lisp
{:name my-package
 :dependencies {proof "file:../path/to/proof"}
 :load-roots ["src" "test"]
 :config {proof {:test-roots ["test"]
                 :namespace "my-package.checkout"
                 :include-sub-namespaces? true
                 :filter "*discount*"
                 :reporter :tree}}}
```

Use a vector to select multiple namespaces:

```lisp
{:config {proof {:namespace ["my-package.checkout"
                             "my-package.profile"]}}}
```

Command-line flags override the `package.edn` values for a single run:

```sh
roo proof --test-root test \
          --filter '*discount*' \
          --namespace my-package.checkout \
          --include-sub-namespaces \
          --reporter tree
```

Use `roo proof --help` to print the proof-specific options without loading or
running tests.

Supported flags are:

- `--help` or `-h`
- `--test-root <path>` or `--test-root=<path>`; repeat it to provide multiple roots
- `--filter <pattern>` or `--filter=<pattern>`
- `--namespace <namespace>` or `--namespace=<namespace>`; repeat it to provide multiple namespaces
- `--include-sub-namespaces`, `--include-sub-namespaces=true|false`, and `--no-include-sub-namespaces`
- `--reporter simple|tree` or `--reporter=simple|tree`
- `--durations`, `--durations=true|false`, and `--no-durations`

Proof tests can also be run manually from Roo code by loading test namespaces and
calling `(run)` yourself.
