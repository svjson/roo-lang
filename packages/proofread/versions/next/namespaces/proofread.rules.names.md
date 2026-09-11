---
layout: reference
title: proofread.rules.names
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.names.html">proofread.rules.names</a>
<ol>
<li><a href="proofread.rules.names/check.html">check</a></li>
<li><a href="proofread.rules.names/check-file.html">check-file</a></li>
<li><a href="proofread.rules.names/rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.names</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.names.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.names

| Symbol                                                | Summary                                     |
| ----------------------------------------------------- | ------------------------------------------- |
| [`check`](proofread.rules.names/check.html)           | Find name diagnostics beneath one AST root. |
| [`check-file`](proofread.rules.names/check-file.html) | Find name diagnostics for one source file.  |
| [`rules`](proofread.rules.names/rules.html)           | Name rule descriptors.                      |

</main>
</div>
