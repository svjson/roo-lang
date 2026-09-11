---
layout: reference
title: boodle.model/package-group
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.model.html">boodle.model</a>
<ol>
<li><a href="catalog.html">catalog</a></li>
<li><a href="entry.html">entry</a></li>
<li><a href="namespace.html">namespace</a></li>
<li><a href="package.html">package</a></li>
<li><a class="active" href="package-group.html">package-group</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.model.html">boodle.model</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">package-group</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.model/package-group.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.model.html">boodle.model</a>/</div>
<h1><code>package-group</code></h1>
</header>

Create a package group for catalog-level navigation.

<hr class="signature-divider">

```roo
(package-group data)
```

<hr class="signature-divider">

## Signature:

```roo
(package-group data)
```

| Arg  | Description                                 |
| ---- | ------------------------------------------- |
| data | Map containing `:title` and `:package-ids`. |

### Returns:

A `:boodle/package-group` map.

</main>
</div>
