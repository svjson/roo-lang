---
layout: reference
title: proofread.rules.higher-order.use-keep
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.higher-order.use-keep.html">proofread.rules.higher-order.use-keep</a>
<ol>
<li><a href="proofread.rules.higher-order.use-keep/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.higher-order.use-keep/check.html">check</a></li>
<li><a href="proofread.rules.higher-order.use-keep/map-filter-p.html">map-filter?</a></li>
<li><a href="proofread.rules.higher-order.use-keep/map-with-conditional-body-p.html">map-with-conditional-body?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.higher-order.use-keep</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.higher-order.use-keep.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.higher-order.use-keep

| Symbol                                                                                                 | Summary                                                    |
| ------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------- |
| [`candidate?`](proofread.rules.higher-order.use-keep/candidate-p.html)                                 | Test whether an AST node can use `keep`.                   |
| [`check`](proofread.rules.higher-order.use-keep/check.html)                                            | Return a `use-keep` diagnostic for matching nodes.         |
| [`map-filter?`](proofread.rules.higher-order.use-keep/map-filter-p.html)                               | Test whether a map consumes a filtered sequence.           |
| [`map-with-conditional-body?`](proofread.rules.higher-order.use-keep/map-with-conditional-body-p.html) | Test whether a map function conditionally returns a value. |

</main>
</div>
