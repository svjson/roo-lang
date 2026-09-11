---
layout: reference
title: proofread.rules.simple-expressions.redundant-empty-default/empty-seq-default?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.redundant-empty-default.html">proofread.rules.simple-expressions.redundant-empty-default</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="empty-map-default-p.html">empty-map-default?</a></li>
<li><a class="active" href="empty-seq-default-p.html">empty-seq-default?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.redundant-empty-default.html">proofread.rules.simple-expressions.redundant-empty-default</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">empty-seq-default?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-empty-default/empty-seq-default-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.redundant-empty-default.html">proofread.rules.simple-expressions.redundant-empty-default</a>/</div>
<h1><code>empty-seq-default?</code></h1>
</header>

Test whether an AST node supplies an empty sequence default.

<hr class="signature-divider">

```roo
(empty-seq-default? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(empty-seq-default? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for `(or value [])`, `(get value key [])`, and
`(get-in value path [])`; otherwise `false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for `(or value [])`, `(get value key [])`, and
`(get-in value path [])`; otherwise `false`.

</main>
</div>
