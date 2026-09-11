---
layout: reference
title: lookup.doc
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.doc.html">lookup.doc</a>
<ol>
<li><a href="lookup.doc/strip-backticks.html">strip-backticks</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.doc</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.doc.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.doc

| Symbol                                               | Summary                                                 |
| ---------------------------------------------------- | ------------------------------------------------------- |
| [`strip-backticks`](lookup.doc/strip-backticks.html) | Trim TEXT and remove one surrounding pair of backticks. |

</main>
</div>
