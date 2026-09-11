---
layout: reference
title: proofread.rules.direct-conditions
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.html">proofread.rules.direct-conditions</a>
<ol>
<li><a href="proofread.rules.direct-conditions/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions/check-node.html">check-node</a></li>
<li><a href="proofread.rules.direct-conditions/covered-child-p.html">covered-child?</a></li>
<li><a href="proofread.rules.direct-conditions/node-diagnostic.html">node-diagnostic</a></li>
<li><a href="proofread.rules.direct-conditions/rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions

| Symbol                                                                      | Summary                                                           |
| --------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| [`check`](proofread.rules.direct-conditions/check.html)                     | Find direct-condition diagnostics beneath one AST root.           |
| [`check-node`](proofread.rules.direct-conditions/check-node.html)           | Find direct-condition diagnostics beneath one AST node.           |
| [`covered-child?`](proofread.rules.direct-conditions/covered-child-p.html)  | Test whether a child is covered by an existing parent diagnostic. |
| [`node-diagnostic`](proofread.rules.direct-conditions/node-diagnostic.html) | Return the direct-condition diagnostic for an AST node.           |
| [`rules`](proofread.rules.direct-conditions/rules.html)                     | Direct-condition rule descriptors.                                |

</main>
</div>
