---
layout: reference
title: proofread.rules.direct-conditions.use-case/case-literal?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a class="active" href="case-literal-p.html">case-literal?</a></li>
<li><a href="check.html">check</a></li>
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
<span aria-current="page">case-literal?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-case/case-literal-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>/</div>
<h1><code>case-literal?</code></h1>
</header>

Test whether an AST node is a literal suitable for `case` dispatch.

<hr class="signature-divider">

```roo
(case-literal? node)
```

<hr class="signature-divider">

## Signature:

```roo
(case-literal? node)
```

| Arg  | Description           |
| ---- | --------------------- |
| node | A `roo.ast` node map. |

### Returns:

`true` when `node` is a string, keyword, number, or character literal;
otherwise `false`.

</main>
</div>
