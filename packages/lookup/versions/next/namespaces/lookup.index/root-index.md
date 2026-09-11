---
layout: reference
title: lookup.index/root-index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.index.html">lookup.index</a>
<ol>
<li><a href="explicit-package.html">explicit-package</a></li>
<li><a href="index-with-extractors.html">index-with-extractors</a></li>
<li><a href="index-with-options.html">index-with-options</a></li>
<li><a class="active" href="root-index.html">root-index</a></li>
<li><a href="source-form-index.html">source-form-index</a></li>
<li><a href="symbol-index.html">symbol-index</a></li>
<li><a href="symbol-index-with-roots.html">symbol-index-with-roots</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.index.html">lookup.index</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">root-index</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.index/root-index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.index.html">lookup.index</a>/</div>
<h1><code>root-index</code></h1>
</header>

Build a symbol index from generic roots.

<hr class="signature-divider">

```roo
(root-index options)
```

<hr class="signature-divider">

## Signature:

```roo
(root-index options)
```

| Arg     | Description                                                                               |
| ------- | ----------------------------------------------------------------------------------------- |
| options | Map containing `:roots`, `:excludes`, `:extractors`, and<br>optional `:package` metadata. |

### Returns:

A `:roo/symbol-index` map.

</main>
</div>
