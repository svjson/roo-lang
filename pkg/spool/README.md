# spool

Frame-scoped context timing reports for Roo programs.

## Collecting timing contexts

Call `spool/reset!` when a new frame or operation begins. `spool/enter!` opens
a timed context, and `spool/leave!` closes the current context. Opening a
context while another is open nests it beneath the current one, producing a
tree in the report.

Metadata supplied to `leave!` is merged with metadata supplied to `enter!`:

```roo
(ns game.frame
  (:require [spool :as spool]))

(spool/reset!)

(spool/enter! :frame {:command :move})

(spool/enter! :visibility)
(update-player-visibility! world player)
(spool/leave! :visibility {:cells (count (:visible-cells player))})

(spool/enter! :messages)
(deliver-visible-messages! world player)
(spool/leave! :messages {:count (count (:delivered-messages player))})

(spool/leave! :frame {:changed? true})
(spool/report!)
```

This produces a report shaped like:

```text
spool
└── :frame 18ms {:command :move, :changed? true}
    ├── :visibility 7ms {:cells 42}
    └── :messages 3ms {:count 2}
```

`leave!` only closes the current context when its key matches. `report!`
returns the complete context tree whether or not it prints it.

## Wrapping a function

`spool/wrap` takes an existing function and returns a measured replacement.
The replacement accepts the same arguments, forwards them to the original
function, and returns its result unchanged. Creating the wrapper does not
collect anything; each call to the replacement records one context.

For example, the application can keep its ordinary function interface:

```roo
(defun calculate-player-visibility [world player opts]
  (calculate-visible-cells world player opts))

(let [calculate-player-visibility*
      (spool/wrap calculate-player-visibility {:frame :visibility})]
  (calculate-player-visibility* world player {:include-hidden? false}))
```

The original binding can also be replaced, leaving every call site unaware of
the measurement:

```roo
(set! [calculate-player-visibility]
      (spool/wrap calculate-player-visibility {:frame :visibility}))

(calculate-player-visibility world player {:include-hidden? false})
```

Entry metadata can be supplied with `:metadata`:

```roo
(set! [calculate-player-visibility]
      (spool/wrap calculate-player-visibility
                  {:frame :visibility
                   :metadata {:component :player-view}}))
```

When metadata depends on the function's return value, supply an
`:exit-metadata` function. Its result is merged into the context metadata when
the context closes:

```roo
(set! [process-batch]
      (spool/wrap process-batch
                  {:frame :batch
                   :metadata {:source :inbox}
                   :exit-metadata
                   (fn [result]
                     {:processed-count (count result)})}))
```

## Configuring reports

### Conditional reporting

Pass a predicate to `report!` to print only reports that meet a condition. The
predicate receives the complete context tree:

```roo
(spool/report!
 (fn [tree]
   (:changed? (:metadata (head (:contexts tree))))))
```

The tree is still returned when the predicate returns `false`.

### Rendering options

Pass an options map to control which parts of the tree are rendered:

```roo
(spool/report! {:only-keys [:visibility]
                :collapse-keys [:visibility]
                :collapse-below-ms 10})
```

- `:only-keys` renders matching contexts and their ancestor paths, omitting
  unrelated branches.
- `:collapse-keys` renders matching contexts without their children.
- `:collapse-below-ms` renders closed contexts faster than the threshold as
  leaves.

A predicate and rendering options can be supplied together:

```roo
(spool/report!
 (fn [tree]
   (:changed? (:metadata (head (:contexts tree)))))
 {:collapse-below-ms 10})
```
