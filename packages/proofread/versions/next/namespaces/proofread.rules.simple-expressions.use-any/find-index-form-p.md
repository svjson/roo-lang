---
layout: reference
title: proofread.rules.simple-expressions.use-any/find-index-form?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-any.html">proofread.rules.simple-expressions.use-any</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="find-first-form-p.html">find-first-form?</a></li>
<li><a class="active" href="find-index-form-p.html">find-index-form?</a></li>
<li><a href="presence-check-p.html">presence-check?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-any.html">proofread.rules.simple-expressions.use-any</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">find-index-form?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-any/find-index-form-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-any.html">proofread.rules.simple-expressions.use-any</a>/</div>
<h1><code>find-index-form?</code></h1>
</header>

Test whether an AST node is a `(find-index value predicate)` form.

<hr class="signature-divider">

```roo
(find-index-form? node)
```

<hr class="signature-divider">

## Signature:

```roo
(find-index-form? node)
```

| Arg  | Description           |
| ---- | --------------------- |
| node | A `roo.ast` node map. |

### Returns:

`true` for three-child `find-index` calls, otherwise `false`.

</main>
</div>
