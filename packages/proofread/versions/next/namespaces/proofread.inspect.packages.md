---
layout: reference
title: proofread.inspect.packages
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.packages.html">proofread.inspect.packages</a>
<ol>
<li><a href="proofread.inspect.packages/root.html">root</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.packages</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.packages.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.packages

| Symbol                                         | Summary                                                |
| ---------------------------------------------- | ------------------------------------------------------ |
| [`root`](proofread.inspect.packages/root.html) | Find the nearest package root at or above a directory. |

</main>
</div>
