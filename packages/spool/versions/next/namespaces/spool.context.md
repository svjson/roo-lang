---
layout: reference
title: spool.context
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="spool.context.html">spool.context</a>
<ol>
<li><a href="spool.context/add-child-at-path.html">add-child-at-path</a></li>
<li><a href="spool.context/child-count-at-path.html">child-count-at-path</a></li>
<li><a href="spool.context/metadata-map.html">metadata-map</a></li>
<li><a href="spool.context/node-at-path.html">node-at-path</a></li>
<li><a href="spool.context/snapshot.html">snapshot</a></li>
<li><a href="spool.context/state.html">state</a></li>
<li><a href="spool.context/update-node-at-path.html">update-node-at-path</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">spool.context</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/spool.context.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# spool.context

| Symbol                                                          | Summary                                             |
| --------------------------------------------------------------- | --------------------------------------------------- |
| [`add-child-at-path`](spool.context/add-child-at-path.html)     | Append a context below a path in a context tree.    |
| [`child-count-at-path`](spool.context/child-count-at-path.html) | Return the number of children below a context path. |
| [`metadata-map`](spool.context/metadata-map.html)               | Merge optional context metadata maps.               |
| [`node-at-path`](spool.context/node-at-path.html)               | Return the context node at a path.                  |
| [`snapshot`](spool.context/snapshot.html)                       | Return the collected context tree.                  |
| [`state`](spool.context/state.html)                             | Mutable context collection state.                   |
| [`update-node-at-path`](spool.context/update-node-at-path.html) | Update a context node at a path.                    |

</main>
</div>
