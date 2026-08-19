# voodoo

Voodoo is a terminal layout and motion package for Roo: session-managed
transient output today, with progress bars, spinners, and box/rule/banner
rendering planned.

## Sessions

A session tracks widgets, supporting adding, updating, and removing them
without the call site having to reason about cursor position, or even
know whether it's writing to a real terminal:

```clojure
(ns app
  (:require [voodoo.session :as session]
            [voodoo.widget :as widget]))

(def s (session/make))
(def h (session/add! s (widget/make {:type :label :text "Discovering proof tests..."})))

(session/update! s h {:text "Discovering proof tests... (42 found)"})
(session/remove! s h)
```

`:type` is just a plain value for the caller's own use (telling widgets
apart, logging about them, and so on) - voodoo never inspects it.

`make` takes an optional `:tty?` flag. Omit it to auto-detect via
`roo.tty/tty?`; pass it explicitly to force terminal mode on or off:

```clojure
(session/make {:tty? false}) ; e.g. CI/log output, no ANSI codes
```

Handles are plain data (an id), not closures - `session` and `handle` are
passed into `update!`/`remove!` explicitly, rather than calling a method
off the handle itself.

## Widgets and non-terminal output

`voodoo.widget/make` builds a widget from the given fields (at minimum
`:type` and `:text`), filling in a **degrade policy**: what a widget
should do when output isn't going to a real terminal (piped to a file,
running in CI, etc). Terminal output always shows a widget's current
state live; the degrade policy only controls what non-terminal output
sees.

```clojure
(widget/make {:type :label :text "Discovering proof tests..."})
```

Four ready-made policies cover the common cases, set via `:degrade` when
the default doesn't fit:

- **`visible-once`** (the default) - shown once when added, then quiet.
  Good for a one-off status line: worth a single line in a log, not a
  scrolling record of every change made to it.
- **`silent`** - never shown outside a terminal, at any point in its
  life. Good for something with no meaningful text form at all, like an
  animated spinner frame.
- **`tty-only`** - shown live in a terminal; entirely absent from
  non-terminal output, as if it never existed. Good for an in-progress
  indicator (`"Running..."`) with a separate final line printed once
  it's done - the in-progress state itself isn't worth logging.
- **`replaced`** - hidden when added, but its first update *is* shown.
  Good for a widget that starts as a placeholder and is updated exactly
  once to its real, final content.

```clojure
(widget/make {:type :status
             :text "Running some-test..."
             :degrade widget/tty-only})
```

`:degrade` can also be given a partial map to change just one part of
the default policy, e.g. `{:degrade {:on-update :visible}}`.

## Multi-line widgets

A widget's `:text` can span multiple lines (separated by `\n`) - for
example, a result line followed by a few lines of detail. Voodoo measures
and redraws multi-line widgets correctly, including when a widget grows
from one line to several between updates.

## Managing many widgets

For a session holding many widgets that finish one at a time - a list of
in-progress items that each resolve to a final state, say - `evict!`
drops a widget from tracking once it has reached its permanent final
content:

```clojure
(session/update! s h final-spec)
(session/evict! s h)
```

`evict!` stops the session from tracking that widget any further. Its
row stays on screen exactly as last drawn, but later updates to *other*
widgets no longer need to redraw it - which matters once a widget's
content is long, or once there are many of them.
