---
layout: reference
title: proofread.rules.simple-expressions.use-empty/empty-count-comparison?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="count-form-argument.html">count-form-argument</a></li>
<li><a class="active" href="empty-count-comparison-p.html">empty-count-comparison?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">empty-count-comparison?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-empty/empty-count-comparison-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>/</div>
<h1><code>empty-count-comparison?</code></h1>
</header>

Test whether an AST node compares a count to zero.

<hr class="signature-divider">

```roo
(empty-count-comparison? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(empty-count-comparison? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for `(= 0 (count value))` and `(= (count value) 0)`, otherwise
`false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for `(= 0 (count value))` and `(= (count value) 0)`, otherwise
`false`.

</main>
</div>
