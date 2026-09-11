---
layout: reference
title: proofread.rules.redundant-functions
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.redundant-functions.html">proofread.rules.redundant-functions</a>
<ol>
<li><a href="proofread.rules.redundant-functions/check.html">check</a></li>
<li><a href="proofread.rules.redundant-functions/rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.redundant-functions</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.redundant-functions.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.redundant-functions

| Symbol                                                    | Summary                                                    |
| --------------------------------------------------------- | ---------------------------------------------------------- |
| [`check`](proofread.rules.redundant-functions/check.html) | Find redundant function diagnostics for a parsed AST root. |
| [`rules`](proofread.rules.redundant-functions/rules.html) | Redundant-function rule descriptors.                       |

</main>
</div>
