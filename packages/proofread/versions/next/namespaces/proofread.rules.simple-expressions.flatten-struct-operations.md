---
layout: reference
title: proofread.rules.simple-expressions.flatten-struct-operations
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.flatten-struct-operations.html">proofread.rules.simple-expressions.flatten-struct-operations</a>
<ol>
<li><a href="proofread.rules.simple-expressions.flatten-struct-operations/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.flatten-struct-operations/operation-chain.html">operation-chain</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.flatten-struct-operations</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.flatten-struct-operations.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.flatten-struct-operations

| Symbol                                                                                                 | Summary                                                            |
| ------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------ |
| [`check`](proofread.rules.simple-expressions.flatten-struct-operations/check.html)                     | Report nested struct operations that should be one variadic call.  |
| [`operation-chain`](proofread.rules.simple-expressions.flatten-struct-operations/operation-chain.html) | Describe a chain of nested calls to one variadic struct operation. |

</main>
</div>
