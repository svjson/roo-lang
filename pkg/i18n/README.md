# i18n

`i18n` provides small translation catalog helpers for Roo programs.

Catalogs are ordinary maps with a default locale and a nested translation table:

```lisp
(ns app.copy
  (:require [i18n.core :as i18n]))

(def copy
  (i18n/catalog :en
                {:en {:greeting "Hello"
                      :checkout "Checkout"}
                 :sv {:greeting "Hej"}}))

(i18n/translate copy :sv :greeting)
; => "Hej"

(i18n/translate copy :sv :checkout)
; => "Checkout"

(i18n/translate copy :sv :missing)
; => ":missing"
```

`translate` first checks the requested locale, then the catalog default locale,
then returns the key text when no translation is available.

Use `message` for exact locale lookup without fallback and `missing?` to check
whether a locale is missing a key.
