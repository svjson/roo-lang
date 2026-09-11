---
layout: reference
title: proofread.rules.direct-conditions.use-unless
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.use-unless.html">proofread.rules.direct-conditions.use-unless</a>
<ol>
<li><a href="proofread.rules.direct-conditions.use-unless/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions.use-unless/if-empty-then-branch-p.html">if-empty-then-branch?</a></li>
<li><a href="proofread.rules.direct-conditions.use-unless/replacement.html">replacement</a></li>
<li><a href="proofread.rules.direct-conditions.use-unless/when-negated-condition-p.html">when-negated-condition?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.use-unless</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-unless.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.use-unless

| Symbol                                                                                                  | Summary                                              |
| ------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| [`check`](proofread.rules.direct-conditions.use-unless/check.html)                                      | Return a `use-unless` diagnostic for matching nodes. |
| [`if-empty-then-branch?`](proofread.rules.direct-conditions.use-unless/if-empty-then-branch-p.html)     | Test whether an `if` form has an empty then branch.  |
| [`replacement`](proofread.rules.direct-conditions.use-unless/replacement.html)                          | Return a rewrite candidate for `use-unless`.         |
| [`when-negated-condition?`](proofread.rules.direct-conditions.use-unless/when-negated-condition-p.html) | Test whether a `when` form negates its condition.    |

</main>
</div>
