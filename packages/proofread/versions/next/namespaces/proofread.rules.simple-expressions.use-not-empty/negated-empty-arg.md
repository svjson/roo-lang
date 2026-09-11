---
layout: reference
title: proofread.rules.simple-expressions.use-not-empty/negated-empty-arg
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="count-empty-arg.html">count-empty-arg</a></li>
<li><a href="direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a class="active" href="negated-empty-arg.html">negated-empty-arg</a></li>
<li><a href="not-empty-count-comparison-p.html">not-empty-count-comparison?</a></li>
<li><a href="not-empty-negation-p.html">not-empty-negation?</a></li>
<li><a href="replacement.html">replacement</a></li>
<li><a href="source-argument.html">source-argument</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">negated-empty-arg</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-not-empty/negated-empty-arg.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>/</div>
<h1><code>negated-empty-arg</code></h1>
</header>

Return the argument to an `empty?` call in a negated form.

<hr class="signature-divider">

```roo
(negated-empty-arg {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(negated-empty-arg {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

The argument to `(empty? ...)`.

- `node`: A matching negated-empty `use-not-empty` AST node.

### Returns:

The argument to `(empty? ...)`.

</main>
</div>
