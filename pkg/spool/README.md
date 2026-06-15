# spool

Frame-scoped context timing reports for Roo programs.

Use `spool/reset!` at the start of a frame, wrap measured work with
`spool/enter!` and `spool/leave!`, and call `spool/report!` at the end of the
frame. `report!` accepts an optional predicate that receives the report tree and
decides whether to print it.

```roo
(spool/reset!)
(spool/enter! :frame {:command :move})
(spool/enter! :visibility)
(spool/leave! :visibility {:cells 42})
(spool/leave! :frame {:changed? true})

(spool/report! (fn [tree]
                 (:changed? (:metadata (head (:contexts tree))))))
```

The report tree is returned from `report!` whether or not the predicate allows
printing.

When printed, reports are rendered as a compact tree:

```text
spool
└── :frame 18ms {:command :move, :changed? true}
    └── :visibility 7ms {:cells 42}
```
