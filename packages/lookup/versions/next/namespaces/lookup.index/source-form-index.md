---
layout: reference
title: lookup.index/source-form-index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.index.html">lookup.index</a>
<ol>
<li><a href="explicit-package.html">explicit-package</a></li>
<li><a href="index-with-extractors.html">index-with-extractors</a></li>
<li><a href="index-with-options.html">index-with-options</a></li>
<li><a href="root-index.html">root-index</a></li>
<li><a class="active" href="source-form-index.html">source-form-index</a></li>
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
<span aria-current="page">source-form-index</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.index/source-form-index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.index.html">lookup.index</a>/</div>
<h1><code>source-form-index</code></h1>
</header>

Build a source-form index for a package.

<hr class="signature-divider">

```roo
(source-form-index package-dir)
```

<hr class="signature-divider">

## Signature:

```roo
(source-form-index package-dir)
```

| Arg         | Description                            |
| ----------- | -------------------------------------- |
| package-dir | Package root containing `package.edn`. |

### Returns:

A `:roo/source-forms` index map with package metadata and parsed top-level
forms.

</main>
</div>
