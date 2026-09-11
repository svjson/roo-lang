---
layout: reference
title: proof.assertion/format-failure-message
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.assertion.html">proof.assertion</a>
<ol>
<li><a href="diff-lines.html">diff-lines</a></li>
<li><a href="equality-fail-message.html">equality-fail-message</a></li>
<li><a class="active" href="format-failure-message.html">format-failure-message</a></li>
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
<span aria-current="page">format-failure-message</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.assertion/format-failure-message.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.assertion.html">proof.assertion</a>/</div>
<h1><code>format-failure-message</code></h1>
</header>

Format a failed assertion message.

<hr class="signature-divider">

```roo
(format-failure-message kind form expected actual)
```

<hr class="signature-divider">

## Signature:

```roo
(format-failure-message kind form expected actual)
```

| Arg      | Description                                                                |
| -------- | -------------------------------------------------------------------------- |
| kind     | Assertion kind keyword. Supported values are `:equality` and<br>`:truthy`. |
| form     | Source text for the assertion expression.                                  |
| expected | Evaluated expected value for equality assertions.                          |
| actual   | Evaluated actual value for equality assertions.                            |

### Returns:

A human-readable failure message.

</main>
</div>
