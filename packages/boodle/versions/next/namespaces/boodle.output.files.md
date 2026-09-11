---
layout: reference
title: boodle.output.files
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.output.files.html">boodle.output.files</a>
<ol>
<li><a href="boodle.output.files/write-bang.html">write!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.output.files</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.output.files.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.output.files

| Symbol                                          | Summary                                               |
| ----------------------------------------------- | ----------------------------------------------------- |
| [`write!`](boodle.output.files/write-bang.html) | Write generated documentation files under OUTPUT-DIR. |

</main>
</div>
