---
layout: reference
title: proofread.rules.direct-conditions.use-if-let
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.use-if-let.html">proofread.rules.direct-conditions.use-if-let</a>
<ol>
<li><a href="proofread.rules.direct-conditions.use-if-let/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.direct-conditions.use-if-let/check.html">check</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.use-if-let</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-if-let.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.use-if-let

| Symbol                                                                        | Summary                                              |
| ----------------------------------------------------------------------------- | ---------------------------------------------------- |
| [`candidate?`](proofread.rules.direct-conditions.use-if-let/candidate-p.html) | Test whether a `let` form should be `if-let`.        |
| [`check`](proofread.rules.direct-conditions.use-if-let/check.html)            | Return a `use-if-let` diagnostic for matching nodes. |

</main>
</div>
