---
layout: reference
title: proofread.rules.simple-expressions.use-type-predicate
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-type-predicate.html">proofread.rules.simple-expressions.use-type-predicate</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/predicates.html">predicates</a></li>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/replacement.html">replacement</a></li>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/type-of-call-p.html">type-of-call?</a></li>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/type-of-comparison.html">type-of-comparison</a></li>
<li><a href="proofread.rules.simple-expressions.use-type-predicate/type-predicate.html">type-predicate</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-type-predicate</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-type-predicate.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-type-predicate

| Symbol                                                                                                | Summary                                                      |
| ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| [`check`](proofread.rules.simple-expressions.use-type-predicate/check.html)                           | Report type-of equality checks with direct predicates.       |
| [`predicates`](proofread.rules.simple-expressions.use-type-predicate/predicates.html)                 | Type names with direct predicate functions.                  |
| [`replacement`](proofread.rules.simple-expressions.use-type-predicate/replacement.html)               | Return replacement source for a type-of equality comparison. |
| [`type-of-call?`](proofread.rules.simple-expressions.use-type-predicate/type-of-call-p.html)          | Test whether an AST node is a one-argument `type-of` call.   |
| [`type-of-comparison`](proofread.rules.simple-expressions.use-type-predicate/type-of-comparison.html) | Return `[value predicate]` for type-of equality comparisons. |
| [`type-predicate`](proofread.rules.simple-expressions.use-type-predicate/type-predicate.html)         | Return the direct predicate for a type name node.            |

</main>
</div>
