---
layout: reference
title: proofread.inspection/diagnostics
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspection.html">proofread.inspection</a>
<ol>
<li><a class="active" href="diagnostics.html">diagnostics</a></li>
<li><a href="file-result.html">file-result</a></li>
<li><a href="parse-error-diagnostic.html">parse-error-diagnostic</a></li>
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
<span aria-current="page">diagnostics</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspection/diagnostics.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspection.html">proofread.inspection</a>/</div>
<h1><code>diagnostics</code></h1>
</header>

Return diagnostics for an input from a native check result.

<hr class="signature-divider">

```roo
(diagnostics file check-result options)
```

<hr class="signature-divider">

## Signature:

```roo
(diagnostics file check-result options)
```

| Arg          | Description                                                    |
| ------------ | -------------------------------------------------------------- |
| file         | Source file identity.                                          |
| check-result | Native source-check result map.                                |
| options      | Source input options passed to `proofread.source/diagnostics`. |

### Returns:

A vector of rule diagnostics, including parse diagnostics when checking fails.

</main>
</div>
