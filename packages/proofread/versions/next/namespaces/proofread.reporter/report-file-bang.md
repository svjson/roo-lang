---
layout: reference
title: proofread.reporter/report-file!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.reporter.html">proofread.reporter</a>
<ol>
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
<a href="../proofread.reporter.html">proofread.reporter</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">report-file!</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.reporter/report-file-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.reporter.html">proofread.reporter</a>/</div>
<h1><code>report-file!</code></h1>
</header>

Report one checked file.

<hr class="signature-divider">

```roo
(report-file! reporter file-result)
```

<hr class="signature-divider">

## Signature:

```roo
(report-file! reporter file-result)
```

| Arg         | Description                                              |
| ----------- | -------------------------------------------------------- |
| reporter    | Reporter map returned by `make`.                         |
| file-result | Map containing file path, diagnostics, and error counts. |

### Returns:

The reporter map with updated reporter state.

</main>
</div>
