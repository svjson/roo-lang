# workbook

`workbook` is a small pure Roo package for controlled application state
updates. It stores an editable value together with patch history, undo/redo
stacks, transactions, and dirty-state metadata.

```clojure
(require [workbook :as workbook])

(def book (workbook/init {:name "Map"}))

(def renamed
  (workbook/set book [:name] "Overworld" {:label "Rename map"}))

(workbook/value renamed)
;; => {:name "Overworld"}

(workbook/value (workbook/undo renamed))
;; => {:name "Map"}
```

Patch helpers create compact forward and inverse changes from the current
value. The first patch vocabulary is deliberately small:

- `:set` replaces the value at a path.
- `:insert` inserts into the vector at a path.
- `:remove` removes from the vector at a path.

Transactions group multiple writes into one history entry:

```clojure
(-> book
    (workbook/begin "Paint tiles")
    (workbook/set [:tiles 0 0] :grass)
    (workbook/set [:tiles 0 1] :grass)
    (workbook/commit))
```

The public namespace is `workbook`. The implementation is split into focused
namespaces:

- `workbook.value` owns the workbook wrapper and revision/dirty metadata.
- `workbook.patch.intent` turns patch intent into concrete patches.
- `workbook.patch.apply` applies and reverts concrete patches.
- `workbook.history` records entries, transactions, undo, and redo.
