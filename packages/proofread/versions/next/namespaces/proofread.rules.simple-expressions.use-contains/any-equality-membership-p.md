---
layout: reference
title: proofread.rules.simple-expressions.use-contains/any-equality-membership?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-contains.html">proofread.rules.simple-expressions.use-contains</a>
<ol>
<li><a href="any-equality-membership-reversed-p.html">any-equality-membership-reversed?</a></li>
<li><a class="active" href="any-equality-membership-p.html">any-equality-membership?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="equality-against-param-p.html">equality-against-param?</a></li>
<li><a href="equality-membership-predicate-p.html">equality-membership-predicate?</a></li>
<li><a href="index-of-form-p.html">index-of-form?</a></li>
<li><a href="presence-check-p.html">presence-check?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-contains.html">proofread.rules.simple-expressions.use-contains</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">any-equality-membership?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-contains/any-equality-membership-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-contains.html">proofread.rules.simple-expressions.use-contains</a>/</div>
<h1><code>any-equality-membership?</code></h1>
</header>

Test whether `any?` performs a simple equality membership scan.

<hr class="signature-divider">

```roo
(any-equality-membership? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(any-equality-membership? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for `(any? values (fn [value] (= needle value)))`, otherwise `false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for `(any? values (fn [value] (= needle value)))`, otherwise `false`.

</main>
</div>
