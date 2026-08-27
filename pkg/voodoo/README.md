# voodoo

Voodoo is a terminal layout, editing, and widget package for Roo. It provides
session-managed transient output, composable editors, and interactive widgets,
including spinners, with progress bars and box/rule/banner rendering planned.

## Editors

`voodoo.editor` dispatches portable input events through ordered editor layers.
Later layers receive the first opportunity to handle an event and can return
`nil` to pass it to a lower layer. This lets an application compose
syntax-aware behavior over ordinary text editing without placing language rules
in Voodoo.

```roo
(def editor
  (voodoo.editor/compose
    voodoo.editor.text/default
    {:layers [{:textmap {"(" insert-balanced-parens}}]}))
```

Each layer can provide `:keymap`, `:textmap`, `:on-text`, and `:on-paste`.
Key and exact-text functions receive the current buffer; general text and paste
functions also receive the event text. A non-`nil` buffer means the event was
handled.

`voodoo.editor.text/default` provides ordinary prompt editing: text and paste
insertion, scalar movement and deletion, Home/End and Control-A/E, word
movement with Control-Left/Right, and Control-K deletion through the end.
`voodoo.widget.prompt/make` uses it unless an explicit `:editor` is supplied.

## Spinners

`voodoo.widget.spinner/make` creates a terminal-only spinner widget with a
label. `tick` returns its next frame, ready to replace the current widget in a
session:

```roo
(ns app
  (:require [voodoo.session :as session]
            [voodoo.widget.spinner :as spinner]))

(def terminal (session/make))
(def indicator (spinner/make {:label "src/app.roo" :shape :line}))
(def handle (session/add! terminal indicator))

(set! [indicator] (spinner/tick indicator))
(session/update! terminal handle indicator)
```

The caller decides when to tick. Voodoo provides two named stock shapes:

- `:dots` — the default clockwise Braille cycle.
- `:line` — the ASCII `/`, `-`, `\`, `|` cycle.

Select one with `:shape`. A custom nonempty `:frames` vector may be supplied
when neither stock shape fits.

## Sessions

A session tracks widgets, supporting adding, updating, and removing them
without the call site having to reason about cursor position, or even
know whether it's writing to a real terminal:

```roo
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

```roo
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

```roo
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

```roo
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

```roo
(session/update! s h final-spec)
(session/evict! s h)
```

`evict!` stops the session from tracking that widget any further. Its
row stays on screen exactly as last drawn, but later updates to *other*
widgets no longer need to redraw it - which matters once a widget's
content is long, or once there are many of them.
