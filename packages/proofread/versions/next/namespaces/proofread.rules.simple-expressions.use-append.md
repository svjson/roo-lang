---
layout: reference
title: proofread.rules.simple-expressions.use-append
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-append.html">proofread.rules.simple-expressions.use-append</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-append/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-append/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-append/replacement.html">replacement</a></li>
<li><a href="proofread.rules.simple-expressions.use-append/single-item-vector-p.html">single-item-vector?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-append</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-append.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-append

| Symbol                                                                                           | Summary                                                                |
| ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------- |
| [`candidate?`](proofread.rules.simple-expressions.use-append/candidate-p.html)                   | Test whether a `concat` call appends exactly one item.                 |
| [`check`](proofread.rules.simple-expressions.use-append/check.html)                              | Report `concat` calls that should use `append`.                        |
| [`replacement`](proofread.rules.simple-expressions.use-append/replacement.html)                  | Return a source replacement for a single-item `concat` append pattern. |
| [`single-item-vector?`](proofread.rules.simple-expressions.use-append/single-item-vector-p.html) | Test whether a node is a vector containing one element.                |

</main>
</div>
