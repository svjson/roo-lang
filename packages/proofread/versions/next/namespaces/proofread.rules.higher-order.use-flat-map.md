---
layout: reference
title: proofread.rules.higher-order.use-flat-map
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.higher-order.use-flat-map.html">proofread.rules.higher-order.use-flat-map</a>
<ol>
<li><a href="proofread.rules.higher-order.use-flat-map/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.higher-order.use-flat-map/check.html">check</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.higher-order.use-flat-map</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.higher-order.use-flat-map.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.higher-order.use-flat-map

| Symbol                                                                     | Summary                                                |
| -------------------------------------------------------------------------- | ------------------------------------------------------ |
| [`candidate?`](proofread.rules.higher-order.use-flat-map/candidate-p.html) | Test whether an AST node can use `flat-map`.           |
| [`check`](proofread.rules.higher-order.use-flat-map/check.html)            | Return a `use-flat-map` diagnostic for matching nodes. |

</main>
</div>
