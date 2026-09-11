---
layout: reference
title: proofread.rules.simple-expressions.use-keyword-lookup/get-default-child?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="defaulted-get-fallback-p.html">defaulted-get-fallback?</a></li>
<li><a class="active" href="get-default-child-p.html">get-default-child?</a></li>
<li><a href="get-form-p.html">get-form?</a></li>
<li><a href="keyword-get-p.html">keyword-get?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">get-default-child?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-keyword-lookup/get-default-child-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>/</div>
<h1><code>get-default-child?</code></h1>
</header>

Test whether `node` is the `get` child in a defaulted `or` form.

<hr class="signature-divider">

```roo
(get-default-child? {:keys [children] :as parent} node)
```

<hr class="signature-divider">

## Signature:

```roo
(get-default-child? {:keys [children] :as parent} node)
```

| Arg                           | Description                 |
| ----------------------------- | --------------------------- |
| {:keys [children] :as parent} |                             |
| node                          | Current `roo.ast` node map. |

### Returns:

`true` for the `(get value key)` child in `(or (get value key) default)`,
otherwise `false`.

- `parent`: Parent `roo.ast` node map, or `nil` for a root node.

### Returns:

`true` for the `(get value key)` child in `(or (get value key) default)`,
otherwise `false`.

</main>
</div>
