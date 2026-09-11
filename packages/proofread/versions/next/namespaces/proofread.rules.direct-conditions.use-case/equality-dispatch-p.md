---
layout: reference
title: proofread.rules.direct-conditions.use-case/equality-dispatch?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="case-literal-p.html">case-literal?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="equality-dispatch-expression.html">equality-dispatch-expression</a></li>
<li><a class="active" href="equality-dispatch-p.html">equality-dispatch?</a></li>
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
<span aria-current="page">equality-dispatch?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-case/equality-dispatch-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>/</div>
<h1><code>equality-dispatch?</code></h1>
</header>

Test whether an AST node compares one expression to one literal.

<hr class="signature-divider">

```roo
(equality-dispatch? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(equality-dispatch? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

`true` for an equality call with exactly one `case` literal operand;
otherwise `false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` for an equality call with exactly one `case` literal operand;
otherwise `false`.

</main>
</div>
