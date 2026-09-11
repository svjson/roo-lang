---
layout: reference
title: wraparoo.delimiter
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="wraparoo.delimiter.html">wraparoo.delimiter</a>
<ol>
<li><a href="wraparoo.delimiter/analyze.html">analyze</a></li>
<li><a href="wraparoo.delimiter/pair-at.html">pair-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wraparoo.delimiter</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/wraparoo.delimiter.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# wraparoo.delimiter

Derive delimiter relationships from Wraparoo's lossless token stream.

`analyze` pairs only properly nested collection delimiters and reports the
remaining openers and closers in source order. `pair-at` selects a pair next
to a Unicode-scalar cursor position, preferring a delimiter immediately
before the cursor over one beginning at it.

### Examples:

```roo
(require '[wraparoo :as wraparoo])
(require '[wraparoo.delimiter :as delimiter])

(def analysis (wraparoo/analyze "([value])"))

(delimiter/pair-at analysis 2)
(:unmatched-delimiters (wraparoo/analyze "(value"))
```

Call `analyze` with tokens from `wraparoo.lex/scan` when composing the lower
level passes directly.

| Symbol                                       | Summary                                                                     |
| -------------------------------------------- | --------------------------------------------------------------------------- |
| [`analyze`](wraparoo.delimiter/analyze.html) | Find structurally valid delimiter pairs and unmatched delimiters in TOKENS. |
| [`pair-at`](wraparoo.delimiter/pair-at.html) | Return the delimiter pair touching scalar cursor POSITION in ANALYSIS.      |

</main>
</div>
