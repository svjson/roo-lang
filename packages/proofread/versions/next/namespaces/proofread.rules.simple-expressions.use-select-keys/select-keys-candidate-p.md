---
layout: reference
title: proofread.rules.simple-expressions.use-select-keys/select-keys-candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-select-keys.html">proofread.rules.simple-expressions.use-select-keys</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="select-keys-candidate-p.html">select-keys-candidate?</a></li>
<li><a href="select-keys-entry-target.html">select-keys-entry-target</a></li>
<li><a href="select-keys-target.html">select-keys-target</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-select-keys.html">proofread.rules.simple-expressions.use-select-keys</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">select-keys-candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-select-keys/select-keys-candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-select-keys.html">proofread.rules.simple-expressions.use-select-keys</a>/</div>
<h1><code>select-keys-candidate?</code></h1>
</header>

Test whether a map literal should be `select-keys`.

<hr class="signature-divider">

```roo
(select-keys-candidate? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(select-keys-candidate? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for map literals such as `{:a (:a value) :b (:b value) :c (:c value)}`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for map literals such as `{:a (:a value) :b (:b value) :c (:c value)}`.

</main>
</div>
