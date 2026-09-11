---
layout: reference
title: proofread.rules.direct-conditions.use-cond-thread-first
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.use-cond-thread-first.html">proofread.rules.direct-conditions.use-cond-thread-first</a>
<ol>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/branch-map-p.html">branch-map?</a></li>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/conditional-empty-map-p.html">conditional-empty-map?</a></li>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/conditional-merge-p.html">conditional-merge?</a></li>
<li><a href="proofread.rules.direct-conditions.use-cond-thread-first/conditional-transform-p.html">conditional-transform?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.use-cond-thread-first</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-cond-thread-first.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.use-cond-thread-first

| Symbol                                                                                                           | Summary                                                          |
| ---------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`branch-map?`](proofread.rules.direct-conditions.use-cond-thread-first/branch-map-p.html)                       | Test whether an `if` branch builds a map value.                  |
| [`candidate?`](proofread.rules.direct-conditions.use-cond-thread-first/candidate-p.html)                         | Test whether an AST node should use `cond->`.                    |
| [`check`](proofread.rules.direct-conditions.use-cond-thread-first/check.html)                                    | Return a `use-cond-thread-first` diagnostic for matching nodes.  |
| [`conditional-empty-map?`](proofread.rules.direct-conditions.use-cond-thread-first/conditional-empty-map-p.html) | Test whether a node is `(if condition map {})`.                  |
| [`conditional-merge?`](proofread.rules.direct-conditions.use-cond-thread-first/conditional-merge-p.html)         | Test whether a `merge` call has conditional empty-map arguments. |
| [`conditional-transform?`](proofread.rules.direct-conditions.use-cond-thread-first/conditional-transform-p.html) | Test whether an `if` form should use `cond->`.                   |

</main>
</div>
