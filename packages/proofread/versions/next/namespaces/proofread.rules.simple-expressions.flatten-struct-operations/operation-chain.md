---
layout: reference
title: proofread.rules.simple-expressions.flatten-struct-operations/operation-chain
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.flatten-struct-operations.html">proofread.rules.simple-expressions.flatten-struct-operations</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="operation-chain.html">operation-chain</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.flatten-struct-operations.html">proofread.rules.simple-expressions.flatten-struct-operations</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">operation-chain</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.flatten-struct-operations/operation-chain.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.flatten-struct-operations.html">proofread.rules.simple-expressions.flatten-struct-operations</a>/</div>
<h1><code>operation-chain</code></h1>
</header>

Describe a chain of nested calls to one variadic struct operation.

<hr class="signature-divider">

```roo
(operation-chain {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(operation-chain {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

A map containing the `:operation`, base `:target`, ordered `:arguments`, and
chain `:depth`, or `nil` when `node` is not a supported struct operation.

- `node`: A `roo.ast` node map.

### Returns:

A map containing the `:operation`, base `:target`, ordered `:arguments`, and
chain `:depth`, or `nil` when `node` is not a supported struct operation.

</main>
</div>
