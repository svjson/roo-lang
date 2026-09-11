---
layout: reference
title: wraparoo.syntax/begin-element
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.syntax.html">wraparoo.syntax</a>
<ol>
<li><a class="active" href="begin-element.html">begin-element</a></li>
<li><a href="classify.html">classify</a></li>
<li><a href="constants.html">constants</a></li>
<li><a href="lexical-ranges.html">lexical-ranges</a></li>
<li><a href="local-name-range.html">local-name-range</a></li>
<li><a href="special-forms.html">special-forms</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.syntax.html">wraparoo.syntax</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">begin-element</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.syntax/begin-element.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.syntax.html">wraparoo.syntax</a>/</div>
<h1><code>begin-element</code></h1>
</header>

Record TOKEN as the next direct element of the current source container.

<hr class="signature-divider">

```roo
(begin-element state token data?)
```

<hr class="signature-divider">

## Signature:

```roo
(begin-element state token data?)
```

| Arg   | Description                                                         |
| ----- | ------------------------------------------------------------------- |
| state | Classification state containing `:stack` and accumulated `:ranges`. |
| token | Token beginning the element.                                        |
| data? | Whether the element occurs in quoted or discarded data context.     |

### Returns:

Updated classification state. Parentheses classify their first unquoted
symbol as a call head, definition forms classify their name, and a definition
string becomes a docstring once a following value or form begins.

</main>
</div>
