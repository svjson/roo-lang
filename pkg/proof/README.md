# proof

`proof` is the canonical test framework for Lisple.

It is intentionally small: tests are ordinary Lisple code, assertions are built
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

The test name is a symbol. The body is ordinary Lisple code.

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
    (world/fixture :small-arena)
    (unit/fixture :player {:at [2 2]})
    (unit/fixture :enemy {:at [3 2]}))

  (when
    (input/press! :right)
    (game/tick! 3)
    (events/drain!))

  (then
    (is (= (unit/state :player) :attacking))))
```

These forms do not introduce a separate execution model. Code inside them runs
as normal Lisple code. They are there for readability and for report/context
structure.

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

The default reporter is composed from ordinary Lisple functions in `proof.core`:

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

In a package, put test namespaces under a test load root and list the entry
points in `package.edn`:

```lisp
{:name my-package
 :version "0.1.0"
 :dependencies {proof "file:../path/to/proof"}
 :load-roots ["src" "test"]
 :test-entry-points [my-package.tests]}
```

Then the test namespace can require `proof.core` and call `run`:

```lisp
(ns my-package.tests
  (:require proof.core
            my-package.core))

(clear!)

(deftest works
  (is (= 42 (my-package.core/answer))))

(run)
```
