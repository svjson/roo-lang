---
layout: reference
title: proof.assertion/diff-lines
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.assertion.html">proof.assertion</a>
<ol>
<li><a class="active" href="diff-lines.html">diff-lines</a></li>
<li><a href="equality-fail-message.html">equality-fail-message</a></li>
<li><a href="format-failure-message.html">format-failure-message</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.assertion.html">proof.assertion</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">diff-lines</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.assertion/diff-lines.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.assertion.html">proof.assertion</a>/</div>
<h1><code>diff-lines</code></h1>
</header>

Return rendered line-by-line diff lines for pretty-printed values.

<hr class="signature-divider">

```roo
(diff-lines expected-lines actual-lines)
```

<hr class="signature-divider">

## Signature:

```roo
(diff-lines expected-lines actual-lines)
```

| Arg            | Description                          |
| -------------- | ------------------------------------ |
| expected-lines | Pretty-printed expected value lines. |
| actual-lines   | Pretty-printed actual value lines.   |

### Returns:

Rendered diff lines with unchanged lines prefixed by two spaces, expected-only
lines prefixed with red `-`, and actual-only lines prefixed with green `+`.

</main>
</div>
