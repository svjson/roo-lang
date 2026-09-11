---
layout: reference
title: boodle.model
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.model.html">boodle.model</a>
<ol>
<li><a href="boodle.model/catalog.html">catalog</a></li>
<li><a href="boodle.model/entry.html">entry</a></li>
<li><a href="boodle.model/namespace.html">namespace</a></li>
<li><a href="boodle.model/package.html">package</a></li>
<li><a href="boodle.model/package-group.html">package-group</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.model</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.model.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.model

| Symbol                                             | Summary                                              |
| -------------------------------------------------- | ---------------------------------------------------- |
| [`catalog`](boodle.model/catalog.html)             | Create a documentation catalog.                      |
| [`entry`](boodle.model/entry.html)                 | Create a symbol documentation entry.                 |
| [`namespace`](boodle.model/namespace.html)         | Create a namespace documentation entry.              |
| [`package`](boodle.model/package.html)             | Create a package documentation entry.                |
| [`package-group`](boodle.model/package-group.html) | Create a package group for catalog-level navigation. |

</main>
</div>
