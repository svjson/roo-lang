---
layout: reference
title: wraparoo
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="wraparoo.html">wraparoo</a>
<ol>
<li><a href="wraparoo/analyze.html">analyze</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wraparoo</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/wraparoo.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# wraparoo

Analyze complete or incomplete Roo source for editors and source tools.

`analyze` is the usual entry point. It preserves the complete token stream
and reports structurally valid delimiter pairs and unmatched delimiters
without evaluating, repairing, or otherwise changing the source. All ranges
are half-open Unicode-scalar offsets into the supplied string.

### Examples:

```roo
(require '[wraparoo :as wraparoo])

(def analysis (wraparoo/analyze "(map values"))

(:tokens analysis)
(:syntax-ranges analysis)
(:delimiter-pairs analysis)
(:unmatched-delimiters analysis)
```

Use `wraparoo.lex/scan` directly when only lexical tokens are needed,
`wraparoo.syntax/classify` directly when tokens are already available, and
`wraparoo.delimiter/pair-at` to find the delimiter pair adjacent to a cursor.

| Symbol                             | Summary                                                                       |
| ---------------------------------- | ----------------------------------------------------------------------------- |
| [`analyze`](wraparoo/analyze.html) | Analyze complete or incomplete Roo SOURCE without evaluating or repairing it. |

</main>
</div>
