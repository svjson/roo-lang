---
layout: reference
title: proofread.rules.simple-expressions.use-update/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>
<ol>
<li><a href="assoc-update-p.html">assoc-update?</a></li>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a class="active" href="check.html">check</a></li>
<li><a href="cond-thread-assoc-update-p.html">cond-thread-assoc-update?</a></li>
<li><a href="cond-thread-step-p.html">cond-thread-step?</a></li>
<li><a href="same-key-lookup-p.html">same-key-lookup?</a></li>
<li><a href="update-value-p.html">update-value?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-update/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>/</div>
<h1><code>check</code></h1>
</header>

Return a `use-update` diagnostic for matching nodes.

<hr class="signature-divider">

```roo
(check parent node options)
```

<hr class="signature-divider">

## Signature:

```roo
(check parent node options)
```

| Arg     | Description                                                                                |
| ------- | ------------------------------------------------------------------------------------------ |
| parent  | Parent `roo.ast` node map, or `nil` for a root node.                                       |
| node    | Current `roo.ast` node map.<br>- `_options`: Ignored effective rule options map, or `nil`. |
| options |                                                                                            |

### Returns:

A diagnostic map when `node` matches this rule, otherwise `nil`.

</main>
</div>
