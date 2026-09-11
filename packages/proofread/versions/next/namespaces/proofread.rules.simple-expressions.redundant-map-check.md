---
layout: reference
title: proofread.rules.simple-expressions.redundant-map-check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.redundant-map-check.html">proofread.rules.simple-expressions.redundant-map-check</a>
<ol>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/guarded-lookup-branch-p.html">guarded-lookup-branch?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/lookup-access-p.html">lookup-access?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/lookup-comparison-p.html">lookup-comparison?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/lookup-with-same-fallback-p.html">lookup-with-same-fallback?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-map-check/map-check-p.html">map-check?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.redundant-map-check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-map-check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.redundant-map-check

| Symbol                                                                                                                  | Summary                                                                     |
| ----------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.redundant-map-check/check.html)                                            | Report redundant `map?` guards before keyword lookup.                       |
| [`guarded-lookup-branch?`](proofread.rules.simple-expressions.redundant-map-check/guarded-lookup-branch-p.html)         | Test whether an `if` branch only reads from a map-guarded value.            |
| [`lookup-access?`](proofread.rules.simple-expressions.redundant-map-check/lookup-access-p.html)                         | Test whether an AST node directly accesses a target through keyword lookup. |
| [`lookup-comparison?`](proofread.rules.simple-expressions.redundant-map-check/lookup-comparison-p.html)                 | Test whether an AST node compares a keyword lookup against another value.   |
| [`lookup-with-same-fallback?`](proofread.rules.simple-expressions.redundant-map-check/lookup-with-same-fallback-p.html) | Test whether an `or` branch reads a lookup with the else fallback.          |
| [`map-check?`](proofread.rules.simple-expressions.redundant-map-check/map-check-p.html)                                 | Test whether an AST node is a `(map? value)` form.                          |

</main>
</div>
