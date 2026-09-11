---
layout: reference
title: proofread.rules.direct-conditions.use-when-let
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.use-when-let.html">proofread.rules.direct-conditions.use-when-let</a>
<ol>
<li><a href="proofread.rules.direct-conditions.use-when-let/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions.use-when-let/contains-form-p.html">contains-form?</a></li>
<li><a href="proofread.rules.direct-conditions.use-when-let/let-candidate-p.html">let-candidate?</a></li>
<li><a href="proofread.rules.direct-conditions.use-when-let/nil-else-if-let-p.html">nil-else-if-let?</a></li>
<li><a href="proofread.rules.direct-conditions.use-when-let/repeated-keyword-lookup-p.html">repeated-keyword-lookup?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.use-when-let</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-when-let.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.use-when-let

| Symbol                                                                                                      | Summary                                                          |
| ----------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`check`](proofread.rules.direct-conditions.use-when-let/check.html)                                        | Return a `use-when-let` diagnostic for matching nodes.           |
| [`contains-form?`](proofread.rules.direct-conditions.use-when-let/contains-form-p.html)                     | Test whether an AST subtree contains a matching form.            |
| [`let-candidate?`](proofread.rules.direct-conditions.use-when-let/let-candidate-p.html)                     | Test whether a `let` form should be `when-let`.                  |
| [`nil-else-if-let?`](proofread.rules.direct-conditions.use-when-let/nil-else-if-let-p.html)                 | Test whether an `if-let` form should be `when-let`.              |
| [`repeated-keyword-lookup?`](proofread.rules.direct-conditions.use-when-let/repeated-keyword-lookup-p.html) | Test whether a `when` form repeats its keyword lookup condition. |

</main>
</div>
