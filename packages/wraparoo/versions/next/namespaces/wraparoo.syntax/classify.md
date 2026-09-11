---
layout: reference
title: wraparoo.syntax/classify
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.syntax.html">wraparoo.syntax</a>
<ol>
<li><a href="begin-element.html">begin-element</a></li>
<li><a class="active" href="classify.html">classify</a></li>
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
<span aria-current="page">classify</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.syntax/classify.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.syntax.html">wraparoo.syntax</a>/</div>
<h1><code>classify</code></h1>
</header>

Classify complete or incomplete Roo TOKENS into typed syntax ranges.

<hr class="signature-divider">

```roo
(classify tokens)
```

<hr class="signature-divider">

## Signature:

```roo
(classify tokens)
```

| Arg    | Description                                            |
| ------ | ------------------------------------------------------ |
| tokens | Lossless token vector returned by `wraparoo.lex/scan`. |

### Returns:

A vector of maps shaped as `{:range {:start n :end n} :role keyword}`.
Lexical ranges precede contextual overlays. Contextual roles include call
heads, special forms, definition names, and docstrings. Call heads are not
emitted inside quoted or discarded forms.

</main>
</div>
