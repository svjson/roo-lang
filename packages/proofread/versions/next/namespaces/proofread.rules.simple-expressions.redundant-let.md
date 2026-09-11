---
layout: reference
title: proofread.rules.simple-expressions.redundant-let
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.redundant-let.html">proofread.rules.simple-expressions.redundant-let</a>
<ol>
<li><a href="proofread.rules.simple-expressions.redundant-let/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let/default-options.html">default-options</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.redundant-let</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-let.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.redundant-let

| Symbol                                                                                     | Summary                                                      |
| ------------------------------------------------------------------------------------------ | ------------------------------------------------------------ |
| [`check`](proofread.rules.simple-expressions.redundant-let/check.html)                     | Return a `redundant-let` diagnostic for single-binding lets. |
| [`default-options`](proofread.rules.simple-expressions.redundant-let/default-options.html) | Return the default redundant-let configuration.              |

</main>
</div>
