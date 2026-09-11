---
layout: reference
title: proofread.rules.direct-conditions.use-case
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.use-case.html">proofread.rules.direct-conditions.use-case</a>
<ol>
<li><a href="proofread.rules.direct-conditions.use-case/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/case-literal-p.html">case-literal?</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/equality-dispatch-expression.html">equality-dispatch-expression</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/equality-dispatch-p.html">equality-dispatch?</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/same-case-chain-p.html">same-case-chain?</a></li>
<li><a href="proofread.rules.direct-conditions.use-case/same-cond-case-chain-p.html">same-cond-case-chain?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.use-case</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-case.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.use-case

| Symbol                                                                                                         | Summary                                                              |
| -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| [`candidate?`](proofread.rules.direct-conditions.use-case/candidate-p.html)                                    | Test whether a conditional form should be `case`.                    |
| [`case-literal?`](proofread.rules.direct-conditions.use-case/case-literal-p.html)                              | Test whether an AST node is a literal suitable for `case` dispatch.  |
| [`check`](proofread.rules.direct-conditions.use-case/check.html)                                               | Return a `use-case` diagnostic for matching nodes.                   |
| [`equality-dispatch-expression`](proofread.rules.direct-conditions.use-case/equality-dispatch-expression.html) | Return the non-literal expression in an equality dispatch condition. |
| [`equality-dispatch?`](proofread.rules.direct-conditions.use-case/equality-dispatch-p.html)                    | Test whether an AST node compares one expression to one literal.     |
| [`same-case-chain?`](proofread.rules.direct-conditions.use-case/same-case-chain-p.html)                        | Test whether an `if` chain dispatches on one expression.             |
| [`same-cond-case-chain?`](proofread.rules.direct-conditions.use-case/same-cond-case-chain-p.html)              | Test whether a `cond` chain dispatches on one expression.            |

</main>
</div>
