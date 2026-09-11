---
layout: reference
title: proofread.rules.simple-expressions.use-keyword-lookup/keyword-get?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="defaulted-get-fallback-p.html">defaulted-get-fallback?</a></li>
<li><a href="get-default-child-p.html">get-default-child?</a></li>
<li><a href="get-form-p.html">get-form?</a></li>
<li><a class="active" href="keyword-get-p.html">keyword-get?</a></li>
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
<span aria-current="page">keyword-get?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-keyword-lookup/keyword-get-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>/</div>
<h1><code>keyword-get?</code></h1>
</header>

Test whether an AST node gets a literal keyword key.

<hr class="signature-divider">

```roo
(keyword-get? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(keyword-get? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for `(get value :key)`, otherwise `false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for `(get value :key)`, otherwise `false`.

</main>
</div>
