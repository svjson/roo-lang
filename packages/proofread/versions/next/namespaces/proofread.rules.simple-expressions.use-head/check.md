---
layout: reference
title: proofread.rules.simple-expressions.use-head/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-head.html">proofread.rules.simple-expressions.use-head</a>
<ol>
<li><a class="active" href="check.html">check</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-head.html">proofread.rules.simple-expressions.use-head</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-head/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-head.html">proofread.rules.simple-expressions.use-head</a>/</div>
<h1><code>check</code></h1>
</header>

Report `(nth value 0)` forms.

<hr class="signature-divider">

```roo
(check parent {:keys [children] :as node} options)
```

<hr class="signature-divider">

## Signature:

```roo
(check parent {:keys [children] :as node} options)
```

| Arg                         | Description                                                                                                                                                  |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| parent                      | Parent `roo.ast` node map, or `nil` for a root node.<br>- `node`: Current `roo.ast` node map.<br>- `_options`: Ignored effective rule options map, or `nil`. |
| {:keys [children] :as node} |                                                                                                                                                              |
| options                     |                                                                                                                                                              |

### Returns:

A diagnostic map when `node` should use `head`, otherwise `nil`.

</main>
</div>
