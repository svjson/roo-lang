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
----------------------------------
proof: 1 passed, 0 failed, 1 total
----------------------------------
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

`given`, `when`, and `then` are available for scenario-style tests:

```lisp
(deftest moving-onto-enemy-triggers-attack
  (given
    {:world (world/fixture :small-arena)
     :player (unit/fixture :player {:at [2 2]})
     :enemy (unit/fixture :enemy {:at [3 2]})})

  (when [{:keys [world player]}]
    (input/press world :right)
    (game/tick world 3)
    (events/drain world))

  (then [events {:keys [player]}]
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

```lisp
(deftest simple-scenario
  (given
    4)

  (when [value]
    (+ value 1))

  (then [result]
    (is (= result 5))))
```

## Results

`run` returns a vector of result maps:

```lisp
[{:name addition :status :pass}
 {:name bad-math
  :status :fail
  :message "Expected 5, got 4."
  :failures [{:message "Expected 5, got 4."}]}]
```

A passing result has:

- `:name`
- `:status :pass`

A failing result has:

- `:name`
- `:status :fail`
- `:message`, the first failure message
- `:failures`, all recorded failures for that test

## Reporting

The default reporter is composed from ordinary Roo functions in `proof.core`:

- `passed?`
- `failed?`
- `passed-results`
- `failed-results`
- `pass-count`
- `fail-count`
- `result-summary`
- `summary-text`
- `summary-divider`
- `report-result`
- `report-results`
- `report-failure`
- `report-failures`
- `report-summary`
- `report`

For example:

```lisp
(let [results (run)
      summary (result-summary results)]
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
                 :filter "*discount*"}}}
```

Proof tests can also be run manually from Roo code by loading test namespaces and
calling `(run)` yourself.
