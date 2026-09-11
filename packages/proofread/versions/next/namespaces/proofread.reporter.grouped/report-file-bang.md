---
layout: reference
title: proofread.reporter.grouped/report-file!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.reporter.grouped.html">proofread.reporter.grouped</a>
<ol>
<li><a href="append-group-value.html">append-group-value</a></li>
<li><a href="group-value.html">group-value</a></li>
<li><a href="group-values.html">group-values</a></li>
<li><a href="grouped-diagnostics.html">grouped-diagnostics</a></li>
<li><a href="indent.html">indent</a></li>
<li><a href="indent-width.html">indent-width</a></li>
<li><a href="lines.html">lines</a></li>
<li><a href="lines-at-level.html">lines-at-level</a></li>
<li><a href="lines-with-options.html">lines-with-options</a></li>
<li><a href="make.html">make</a></li>
<li><a class="active" href="report-file-bang.html">report-file!</a></li>
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
<span aria-current="page">report-file!</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.reporter.grouped/report-file-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.reporter.grouped.html">proofread.reporter.grouped</a>/</div>
<h1><code>report-file!</code></h1>
</header>

Collect diagnostics for grouped output.

<hr class="signature-divider">

```roo
(report-file! state file-result)
```

<hr class="signature-divider">

## Signature:

```roo
(report-file! state file-result)
```

| Arg         | Description                                       |
| ----------- | ------------------------------------------------- |
| state       | Reporter state map.                               |
| file-result | Map containing `:diagnostics` for a checked file. |

### Returns:

Updated reporter state.

</main>
</div>
