# voodoo

Voodoo is a terminal layout, editing, and widget package for Roo. It provides
session-managed transient output, composable editors, and interactive widgets,
including spinners, with progress bars and box/rule/banner rendering planned.

## Editors

### Use the default text editor

Prompt widgets provide conventional text editing by default, so ordinary use
does not require constructing an editor:

```roo
(ns app
  (:require [voodoo.widget.prompt :as prompt]))

(prompt/make {:prompt "roo> "
              :value "(+ 1 2)"})
```

The default editor supports text and paste insertion, scalar movement and
deletion, Home/End and Control-A/E, word movement with Control-Left/Right, and
Control-K deletion through the end of the line.

### Compose editing behavior

Pass `:editor` when a widget needs application-specific behavior. An explicit
editor replaces the widget default, so compose additions over
`voodoo.editor.text/default` to retain conventional editing:

```roo
(ns app
  (:require [voodoo.editor :as editor]
            [voodoo.editor.text :as text-editor]
            [voodoo.text.buffer :as buffer]
            [voodoo.widget.prompt :as prompt]))

(def application-editor
  (editor/compose text-editor/default
    {:layers [{:textmap {"(" (fn [text-buffer]
                               (-> text-buffer
                                   (buffer/insert "()")
                                   (buffer/move-left))}}}]}))

(prompt/make {:prompt "roo> "
              :editor application-editor})
```

Editors contain ordered `:layers`. A layer may handle exact chords with
`:keymap`, exact inserted text with `:textmap`, general text with `:on-text`, or
pastes with `:on-paste`. Later layers receive the first opportunity to handle
input. `:keymap` and `:textmap` functions receive the current buffer; general
text and paste functions also receive the event text. Returning `nil` lets the
event continue to a lower layer.

### Present decorated text

Presentation is composed separately from input behavior. Applications can add
semantic source ranges, map them to visual styles, and choose how those styles
are rendered:

```roo
(def presented-editor
  (editor/compose
    text-editor/default
    {:paint paint-styles
     :layers [{:decorations source-decorations}
              {:styling application-styling}]}))

(prompt/make {:prompt "roo> "
              :editor presented-editor})
```

`source-decorations`, `application-styling`, and `paint-styles` above are
functions supplied by the application.

A `:decorations` function receives the logical buffer and returns typed source
ranges. A `:styling` function receives the value, cursor position, and combined
decorations, then returns concrete ranges carrying application-defined styles.
Overlapping styles compose in layer order. The top-level `:paint` function
receives each final style vector and text run; without it, text stays plain.

Prompt widgets automatically render both the complete value and the prefix
before the logical cursor. Presentation output therefore never enters the
editable buffer, history, or submitted value. See the
[presentation namespace](src/editor/presentation.roo) for the range and
rendering operations.

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
