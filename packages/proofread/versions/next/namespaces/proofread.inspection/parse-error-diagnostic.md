---
layout: reference
title: proofread.inspection/parse-error-diagnostic
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspection.html">proofread.inspection</a>
<ol>
<li><a href="diagnostics.html">diagnostics</a></li>
<li><a href="file-result.html">file-result</a></li>
<li><a class="active" href="parse-error-diagnostic.html">parse-error-diagnostic</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspection.html">proofread.inspection</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">parse-error-diagnostic</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspection/parse-error-diagnostic.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspection.html">proofread.inspection</a>/</div>
<h1><code>parse-error-diagnostic</code></h1>
</header>

Build a diagnostic for a parse or read error.

<hr class="signature-divider">

```roo
(parse-error-diagnostic error)
```

<hr class="signature-divider">

## Signature:

```roo
(parse-error-diagnostic error)
```

| Arg   | Description                                                     |
| ----- | --------------------------------------------------------------- |
| error | Error detail map returned by Proofread's native source checker. |

### Returns:

A diagnostic map with `:error` severity.

</main>
</div>
