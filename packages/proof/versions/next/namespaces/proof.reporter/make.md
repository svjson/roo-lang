---
layout: reference
title: proof.reporter/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.html">proof.reporter</a>
<ol>
<li><a class="active" href="make.html">make</a></li>
<li><a href="print-bang.html">print!</a></li>
<li><a href="result-summary.html">result-summary</a></li>
<li><a href="status-count.html">status-count</a></li>
<li><a href="summary-text.html">summary-text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.reporter.html">proof.reporter</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.html">proof.reporter</a>/</div>
<h1><code>make</code></h1>
</header>

<hr class="signature-divider">

```roo
(make options tests-to-run)
```

<hr class="signature-divider">

## Signature:

```roo
(make options tests-to-run)
```

| Arg          | Description |
| ------------ | ----------- |
| options      |             |
| tests-to-run |             |

</main>
</div>
