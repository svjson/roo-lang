---
layout: reference
title: proofread.reporter.grouped/group-values
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.reporter.grouped.html">proofread.reporter.grouped</a>
<ol>
<li><a href="append-group-value.html">append-group-value</a></li>
<li><a href="group-value.html">group-value</a></li>
<li><a class="active" href="group-values.html">group-values</a></li>
<li><a href="grouped-diagnostics.html">grouped-diagnostics</a></li>
<li><a href="indent.html">indent</a></li>
<li><a href="indent-width.html">indent-width</a></li>
<li><a href="lines.html">lines</a></li>
<li><a href="lines-at-level.html">lines-at-level</a></li>
<li><a href="lines-with-options.html">lines-with-options</a></li>
<li><a href="make.html">make</a></li>
<li><a href="report-file-bang.html">report-file!</a></li>
<li><a href="report-summary-bang.html">report-summary!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.reporter.grouped.html">proofread.reporter.grouped</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">group-values</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.reporter.grouped/group-values.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.reporter.grouped.html">proofread.reporter.grouped</a>/</div>
<h1><code>group-values</code></h1>
</header>

Return distinct grouping values for diagnostics.

<hr class="signature-divider">

```roo
(group-values diagnostics criterion)
```

<hr class="signature-divider">

## Signature:

```roo
(group-values diagnostics criterion)
```

| Arg         | Description                 |
| ----------- | --------------------------- |
| diagnostics | Diagnostic maps.            |
| criterion   | Grouping criterion keyword. |

### Returns:

Distinct group values in first-seen order.

</main>
</div>
