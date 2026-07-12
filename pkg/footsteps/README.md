# footsteps

`footsteps` is a pure Roo library for cooperative, resumable work.

It represents long-running work as explicit data, advances it in bounded slices,
and reports monotonic progress even when work discovers and schedules more work
while running.

Step results may include a nested progress report:

```roo
{:complete? false
 :progress {:progress-value 25
            :progress-max 100
            :label "Building roads"}}
```

The report is projected into the running step's allocated progress scope. This
lets application code wrap another batched operation, including another
`footsteps` work value, without coupling `footsteps` execution to that nested
operation.

Application code that owns an outer progress scope can project nested progress
explicitly with `project-progress`:

```roo
(footsteps/project-progress
 {:id :setup/current-step
  :start 2
  :end 3
  :progress-max 10
  :label "Preparing area"}
 {:progress-value 25
  :progress-max 100
  :label "Building roads"})
```

The result is a normal progress report in the outer scope, with the child label
and step id when the child report provides them.
