---
layout: reference
title: wraparoo.syntax/local-name-range
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.syntax.html">wraparoo.syntax</a>
<ol>
<li><a href="begin-element.html">begin-element</a></li>
<li><a href="classify.html">classify</a></li>
<li><a href="constants.html">constants</a></li>
<li><a href="lexical-ranges.html">lexical-ranges</a></li>
<li><a class="active" href="local-name-range.html">local-name-range</a></li>
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
<span aria-current="page">local-name-range</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.syntax/local-name-range.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.syntax.html">wraparoo.syntax</a>/</div>
<h1><code>local-name-range</code></h1>
</header>

Return the local-name portion of symbol TOKEN.

<hr class="signature-divider">

```roo
(local-name-range token)
```

<hr class="signature-divider">

## Signature:

```roo
(local-name-range token)
```

| Arg   | Description                                   |
| ----- | --------------------------------------------- |
| token | Symbol token returned by `wraparoo.lex/scan`. |

### Returns:

TOKEN's complete range when unqualified, or the range beginning at the slash
and ending after the local name when qualified.

</main>
</div>
