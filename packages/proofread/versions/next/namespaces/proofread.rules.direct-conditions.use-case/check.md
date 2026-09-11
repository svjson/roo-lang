---
layout: reference
title: proofread.rules.direct-conditions.use-case/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="case-literal-p.html">case-literal?</a></li>
<li><a class="active" href="check.html">check</a></li>
<li><a href="equality-dispatch-expression.html">equality-dispatch-expression</a></li>
<li><a href="equality-dispatch-p.html">equality-dispatch?</a></li>
<li><a href="same-case-chain-p.html">same-case-chain?</a></li>
<li><a href="same-cond-case-chain-p.html">same-cond-case-chain?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-case/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>/</div>
<h1><code>check</code></h1>
</header>

Return a `use-case` diagnostic for matching nodes.

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

A diagnostic map when `node` should use `case`, otherwise `nil`.

</main>
</div>
