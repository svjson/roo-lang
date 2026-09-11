---
layout: reference
title: boodle.format.lookup/symbol-index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.format.lookup.html">boodle.format.lookup</a>
<ol>
<li><a href="append-name.html">append-name</a></li>
<li><a href="catalog.html">catalog</a></li>
<li><a href="entries-in-namespace.html">entries-in-namespace</a></li>
<li><a href="lookup-namespace.html">lookup-namespace</a></li>
<li><a href="namespace-entry.html">namespace-entry</a></li>
<li><a href="namespace-names.html">namespace-names</a></li>
<li><a href="package-entry.html">package-entry</a></li>
<li><a href="package-id.html">package-id</a></li>
<li><a href="symbol-entry.html">symbol-entry</a></li>
<li><a class="active" href="symbol-index.html">symbol-index</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.format.lookup.html">boodle.format.lookup</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">symbol-index</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.format.lookup/symbol-index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.format.lookup.html">boodle.format.lookup</a>/</div>
<h1><code>symbol-index</code></h1>
</header>

Return the symbol index portion of a lookup index artifact.

<hr class="signature-divider">

```roo
(symbol-index index)
```

<hr class="signature-divider">

## Signature:

```roo
(symbol-index index)
```

| Arg   | Description                                             |
| ----- | ------------------------------------------------------- |
| index | A `:roo/symbol-index` map or combined `:roo/index` map. |

### Returns:

The symbol index map that should be imported into a Boodle catalog.

</main>
</div>
