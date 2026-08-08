# voodoo

Voodoo is a terminal layout and motion package for Roo: session-managed
transient output today, with progress bars, spinners, and box/rule/banner
rendering planned.

## Sessions

A session tracks widgets (currently just `:label`) and lets you add,
update, and remove them without the call site having to reason about
cursor position:

```clojure
(ns app
  (:require [voodoo.session :as session]))

(def s (session/make))
(def h (session/add! s {:type :label :text "Discovering proof tests..."}))

(session/update! s h {:type :label :text "Discovering proof tests... (42 found)"})
(session/remove! s h)
```

`make` takes an optional `:tty?` flag. Omit it to auto-detect (currently a
stub that always resolves `true` - see the tty/ANSI-capability detection
roadmap item in the voodoo docs); pass it explicitly to force terminal mode
on or off:

```clojure
(session/make {:tty? false}) ; e.g. CI/log output, no ANSI codes
```

Handles are plain data (an id), not closures - pass `session` and
`handle` into `update!`/`remove!` explicitly rather than calling a method
off the handle itself.

## Limitations (v1)

- `:label` is the only widget type; it is always a single line.
- `update!`/`remove!` only correctly reflow the terminal when the session's
  own `redraw!`/`add!`/`update!`/`remove!` calls are the only thing writing
  to those rows - interleaving raw `prn` calls with an active session's
  rows will desync the display.
- Removing the bottom-most (or only) widget in tty mode leaves one blank
  row behind rather than reclaiming it. See `remove!`'s docstring in
  `src/voodoo/session.roo` for why.
