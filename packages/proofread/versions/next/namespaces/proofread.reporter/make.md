---
layout: reference
title: proofread.reporter/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.reporter.html">proofread.reporter</a>
<ol>
<li><a class="active" href="make.html">make</a></li>
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
<a href="../proofread.reporter.html">proofread.reporter</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.reporter/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.reporter.html">proofread.reporter</a>/</div>
<h1><code>make</code></h1>
</header>

Create a proofread reporter.

<hr class="signature-divider">

```roo
(make options files)
```

<hr class="signature-divider">

## Signature:

```roo
(make options files)
```

| Arg     | Description                                                  |
| ------- | ------------------------------------------------------------ |
| options | Reporter options map. `:reporter` selects the reporter kind. |
| files   | Files selected for the proofread run.                        |

### Returns:

A reporter map containing state and callback functions.

</main>
</div>
