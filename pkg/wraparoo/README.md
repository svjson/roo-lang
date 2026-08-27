# wraparoo

Wraparoo analyzes and transforms Roo source for editors and source tools. It is
frontend-neutral: its APIs accept source text and Unicode-scalar positions and
return plain tokens, syntax roles, delimiter relationships, and source edits.

Wraparoo has no terminal, widget, input-device, REPL, or presentation concepts.
Applications compose it with their own editor and rendering systems.

## Analysis

```roo
(require '[wraparoo :as wraparoo])
(require '[wraparoo.delimiter :as delimiter])

(def analysis (wraparoo/analyze "(map values"))

(:tokens analysis)
(:syntax-ranges analysis)
(:delimiter-pairs analysis)
(:unmatched-delimiters analysis)

(delimiter/pair-at analysis 4)
```

Token and delimiter ranges are half-open Unicode-scalar offsets into the
original source. Tokens retain their exact source text, including whitespace,
comments, and incomplete literals. Syntax ranges classify comments, strings,
characters, keywords, qualifiers, constants, call heads, special forms,
definition names, and docstrings without choosing visual styles.
