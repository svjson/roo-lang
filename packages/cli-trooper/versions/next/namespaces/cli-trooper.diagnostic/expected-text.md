---
layout: reference
title: cli-trooper.diagnostic/expected-text
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<ol>
<li><a class="active" href="expected-text.html">expected-text</a></li>
<li><a href="line.html">line</a></li>
<li><a href="lines.html">lines</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">expected-text</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.diagnostic/expected-text.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>/</div>
<h1><code>expected-text</code></h1>
</header>

Format an expected diagnostic value for human-readable output.

<hr class="signature-divider">

```roo
(expected-text expected)
```

<hr class="signature-divider">

## Signature:

```roo
(expected-text expected)
```

| Arg      | Description                                       |
| -------- | ------------------------------------------------- |
| expected | One expected value or a sequence of alternatives. |

### Returns:

A comma-separated string for a sequence, or the string representation of one
value.

</main>
</div>
