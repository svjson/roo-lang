---
layout: reference
title: proofread.reporter.simple/report-summary!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.reporter.simple.html">proofread.reporter.simple</a>
<ol>
<li><a href="make.html">make</a></li>
<li><a href="report-file-bang.html">report-file!</a></li>
<li><a class="active" href="report-summary-bang.html">report-summary!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.reporter.simple.html">proofread.reporter.simple</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">report-summary!</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.reporter.simple/report-summary-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.reporter.simple.html">proofread.reporter.simple</a>/</div>
<h1><code>report-summary!</code></h1>
</header>

Print the final proofread summary.

<hr class="signature-divider">

```roo
(report-summary! state summary)
```

<hr class="signature-divider">

## Signature:

```roo
(report-summary! state summary)
```

| Arg     | Description                                                 |
| ------- | ----------------------------------------------------------- |
| state   | Reporter state map.                                         |
| summary | Map containing `:files`, `:warnings`, and `:errors` counts. |

### Returns:

`summary`.

</main>
</div>
