# soot

Soot is a small ANSI SGR styling package for Roo strings.

It returns strings. Roo's `prn` prints string values as raw text, so ANSI
sequences produced by Soot pass through to stdout.

```clojure
(ns app
  (:require [soot :as soot]))

(prn (soot/red "error:") "missing file")
(prn (soot/paint [:bold :green] "ok"))
(prn (soot/fg 208 "warning"))
```

Soot does not detect terminal support. Pass `{:enabled? false}` to return plain
text from styling functions:

```clojure
(soot/paint [:bold :red] "error" {:enabled? false})
```

For most composition, prefer a vector of styles:

```clojure
(soot/paint [:bold :underline :green] "ready")
```

Nested independently styled strings are still plain strings. An inner color
close code can end an outer color before the outer string is complete.
