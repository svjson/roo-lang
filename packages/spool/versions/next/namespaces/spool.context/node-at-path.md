---
layout: reference
title: spool.context/node-at-path
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.context.html">spool.context</a>
<ol>
<li><a href="add-child-at-path.html">add-child-at-path</a></li>
<li><a href="child-count-at-path.html">child-count-at-path</a></li>
<li><a href="metadata-map.html">metadata-map</a></li>
<li><a class="active" href="node-at-path.html">node-at-path</a></li>
<li><a href="snapshot.html">snapshot</a></li>
<li><a href="state.html">state</a></li>
<li><a href="update-node-at-path.html">update-node-at-path</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../spool.context.html">spool.context</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">node-at-path</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool.context/node-at-path.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.context.html">spool.context</a>/</div>
<h1><code>node-at-path</code></h1>
</header>

Return the context node at a path.

<hr class="signature-divider">

```roo
(node-at-path nodes path)
```

<hr class="signature-divider">

## Signature:

```roo
(node-at-path nodes path)
```

| Arg   | Description                              |
| ----- | ---------------------------------------- |
| nodes | Context nodes at the current tree level. |
| path  | Child indexes leading to the context.    |

### Returns:

The selected context node.

</main>
</div>
