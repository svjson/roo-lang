---
layout: reference
title: proofread.reporter.simple
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.reporter.simple.html">proofread.reporter.simple</a>
<ol>
<li><a href="proofread.reporter.simple/make.html">make</a></li>
<li><a href="proofread.reporter.simple/report-file-bang.html">report-file!</a></li>
<li><a href="proofread.reporter.simple/report-summary-bang.html">report-summary!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.reporter.simple</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.reporter.simple.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.reporter.simple

| Symbol                                                                  | Summary                                          |
| ----------------------------------------------------------------------- | ------------------------------------------------ |
| [`make`](proofread.reporter.simple/make.html)                           | Create the default streaming proofread reporter. |
| [`report-file!`](proofread.reporter.simple/report-file-bang.html)       | Print diagnostics for one checked file.          |
| [`report-summary!`](proofread.reporter.simple/report-summary-bang.html) | Print the final proofread summary.               |

</main>
</div>
