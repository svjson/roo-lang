---
layout: reference
title: proofread.rules.simple-expressions.use-append/single-item-vector?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-append.html">proofread.rules.simple-expressions.use-append</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="replacement.html">replacement</a></li>
<li><a class="active" href="single-item-vector-p.html">single-item-vector?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-append.html">proofread.rules.simple-expressions.use-append</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">single-item-vector?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-append/single-item-vector-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-append.html">proofread.rules.simple-expressions.use-append</a>/</div>
<h1><code>single-item-vector?</code></h1>
</header>

Test whether a node is a vector containing one element.

<hr class="signature-divider">

```roo
(single-item-vector? node)
```

<hr class="signature-divider">

## Signature:

```roo
(single-item-vector? node)
```

| Arg  | Description           |
| ---- | --------------------- |
| node | A `roo.ast` node map. |

### Returns:

`true` when the node is a single-item vector, otherwise `false`.

</main>
</div>
