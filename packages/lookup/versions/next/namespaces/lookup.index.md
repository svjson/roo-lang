---
layout: reference
title: lookup.index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.index.html">lookup.index</a>
<ol>
<li><a href="lookup.index/explicit-package.html">explicit-package</a></li>
<li><a href="lookup.index/index-with-extractors.html">index-with-extractors</a></li>
<li><a href="lookup.index/index-with-options.html">index-with-options</a></li>
<li><a href="lookup.index/root-index.html">root-index</a></li>
<li><a href="lookup.index/source-form-index.html">source-form-index</a></li>
<li><a href="lookup.index/symbol-index.html">symbol-index</a></li>
<li><a href="lookup.index/symbol-index-with-roots.html">symbol-index-with-roots</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.index</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.index

| Symbol                                                                 | Summary                                                               |
| ---------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`explicit-package`](lookup.index/explicit-package.html)               | Return explicit package metadata from OPTIONS.                        |
| [`index-with-extractors`](lookup.index/index-with-extractors.html)     | Build an index using the requested extractors.                        |
| [`index-with-options`](lookup.index/index-with-options.html)           | Build an index using parsed CLI options.                              |
| [`root-index`](lookup.index/root-index.html)                           | Build a symbol index from generic roots.                              |
| [`source-form-index`](lookup.index/source-form-index.html)             | Build a source-form index for a package.                              |
| [`symbol-index`](lookup.index/symbol-index.html)                       | Build a symbol index for a package.                                   |
| [`symbol-index-with-roots`](lookup.index/symbol-index-with-roots.html) | Build a package symbol index, optionally enriched with generic roots. |

</main>
</div>
