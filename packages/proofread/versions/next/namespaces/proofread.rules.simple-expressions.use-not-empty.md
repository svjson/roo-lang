---
layout: reference
title: proofread.rules.simple-expressions.use-not-empty
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-not-empty/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/count-empty-arg.html">count-empty-arg</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/negated-empty-arg.html">negated-empty-arg</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/not-empty-count-comparison-p.html">not-empty-count-comparison?</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/not-empty-negation-p.html">not-empty-negation?</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/replacement.html">replacement</a></li>
<li><a href="proofread.rules.simple-expressions.use-not-empty/source-argument.html">source-argument</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-not-empty</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-not-empty.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-not-empty

| Symbol                                                                                                              | Summary                                                                         |
| ------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-not-empty/check.html)                                              | Report positive count checks and negated `empty?` calls.                        |
| [`count-empty-arg`](proofread.rules.simple-expressions.use-not-empty/count-empty-arg.html)                          | Return the argument for a count-based not-empty comparison.                     |
| [`direct-unless-candidate?`](proofread.rules.simple-expressions.use-not-empty/direct-unless-candidate-p.html)       | Test whether this negated expression should be handled by `use-unless` instead. |
| [`negated-empty-arg`](proofread.rules.simple-expressions.use-not-empty/negated-empty-arg.html)                      | Return the argument to an `empty?` call in a negated form.                      |
| [`not-empty-count-comparison?`](proofread.rules.simple-expressions.use-not-empty/not-empty-count-comparison-p.html) | Test whether an AST node compares a count as greater than zero.                 |
| [`not-empty-negation?`](proofread.rules.simple-expressions.use-not-empty/not-empty-negation-p.html)                 | Test whether an AST node negates an `empty?` call.                              |
| [`replacement`](proofread.rules.simple-expressions.use-not-empty/replacement.html)                                  | Return the replacement for `use-not-empty?`.                                    |
| [`source-argument`](proofread.rules.simple-expressions.use-not-empty/source-argument.html)                          | Return the replacement source argument for `not-empty?`.                        |

</main>
</div>
