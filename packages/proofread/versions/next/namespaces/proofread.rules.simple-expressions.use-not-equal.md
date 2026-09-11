---
layout: reference
title: proofread.rules.simple-expressions.use-not-equal
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-not-equal.html">proofread.rules.simple-expressions.use-not-equal</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-not-equal/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-equal/direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-equal/not-equals-negation-p.html">not-equals-negation?</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-equal/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-not-equal</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-not-equal.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-not-equal

| Symbol                                                                                                        | Summary                                                                         |
| ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-not-equal/check.html)                                        | Report negated equality checks.                                                 |
| [`direct-unless-candidate?`](proofread.rules.simple-expressions.use-not-equal/direct-unless-candidate-p.html) | Test whether this negated expression should be handled by `use-unless` instead. |
| [`not-equals-negation?`](proofread.rules.simple-expressions.use-not-equal/not-equals-negation-p.html)         | Test whether an AST node negates an equality call.                              |
| [`replacement`](proofread.rules.simple-expressions.use-not-equal/replacement.html)                            | Return the replacement for `(not (= left right))`.                              |

</main>
</div>
