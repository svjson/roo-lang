---
layout: reference
title: proofread.inspection
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspection.html">proofread.inspection</a>
<ol>
<li><a href="proofread.inspection/diagnostics.html">diagnostics</a></li>
<li><a href="proofread.inspection/file-result.html">file-result</a></li>
<li><a href="proofread.inspection/parse-error-diagnostic.html">parse-error-diagnostic</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspection</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspection.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspection

| Symbol                                                                       | Summary                                                     |
| ---------------------------------------------------------------------------- | ----------------------------------------------------------- |
| [`diagnostics`](proofread.inspection/diagnostics.html)                       | Return diagnostics for an input from a native check result. |
| [`file-result`](proofread.inspection/file-result.html)                       | Inspect one file or in-memory source document.              |
| [`parse-error-diagnostic`](proofread.inspection/parse-error-diagnostic.html) | Build a diagnostic for a parse or read error.               |

</main>
</div>
