---
layout: reference
title: proof.assertion/equality-fail-message
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.assertion.html">proof.assertion</a>
<ol>
<li><a href="diff-lines.html">diff-lines</a></li>
<li><a class="active" href="equality-fail-message.html">equality-fail-message</a></li>
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
<span aria-current="page">equality-fail-message</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.assertion/equality-fail-message.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.assertion.html">proof.assertion</a>/</div>
<h1><code>equality-fail-message</code></h1>
</header>

Format an equality assertion failure message.

<hr class="signature-divider">

```roo
(equality-fail-message form expected actual)
```

<hr class="signature-divider">

## Signature:

```roo
(equality-fail-message form expected actual)
```

| Arg      | Description                               |
| -------- | ----------------------------------------- |
| form     | Source text for the assertion expression. |
| expected | Evaluated expected value.                 |
| actual   | Evaluated actual value.                   |

### Returns:

A human-readable failure message. Multi-line pretty values include a colored
line diff.

</main>
</div>
