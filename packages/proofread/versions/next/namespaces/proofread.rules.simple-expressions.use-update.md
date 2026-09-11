---
layout: reference
title: proofread.rules.simple-expressions.use-update
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-update/assoc-update-p.html">assoc-update?</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/cond-thread-assoc-update-p.html">cond-thread-assoc-update?</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/cond-thread-step-p.html">cond-thread-step?</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/same-key-lookup-p.html">same-key-lookup?</a></li>
<li><a href="proofread.rules.simple-expressions.use-update/update-value-p.html">update-value?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-update</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-update.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-update

| Symbol                                                                                                       | Summary                                                        |
| ------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------- |
| [`assoc-update?`](proofread.rules.simple-expressions.use-update/assoc-update-p.html)                         | Test whether an `assoc` rebuilds a key from its current value. |
| [`candidate?`](proofread.rules.simple-expressions.use-update/candidate-p.html)                               | Test whether an AST node can use `update`.                     |
| [`check`](proofread.rules.simple-expressions.use-update/check.html)                                          | Return a `use-update` diagnostic for matching nodes.           |
| [`cond-thread-assoc-update?`](proofread.rules.simple-expressions.use-update/cond-thread-assoc-update-p.html) | Test whether a `cond->` assoc step should use `update`.        |
| [`cond-thread-step?`](proofread.rules.simple-expressions.use-update/cond-thread-step-p.html)                 | Test whether `node` is a step form in `parent`.                |
| [`same-key-lookup?`](proofread.rules.simple-expressions.use-update/same-key-lookup-p.html)                   | Test whether a node reads `key` from `target-name`.            |
| [`update-value?`](proofread.rules.simple-expressions.use-update/update-value-p.html)                         | Test whether a value expression updates a key's current value. |

</main>
</div>
