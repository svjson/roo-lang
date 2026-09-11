---
layout: reference
title: proofread.rules.registry
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.registry.html">proofread.rules.registry</a>
<ol>
<li><a href="proofread.rules.registry/check.html">check</a></li>
<li><a href="proofread.rules.registry/check-node.html">check-node</a></li>
<li><a href="proofread.rules.registry/descriptors.html">descriptors</a></li>
<li><a href="proofread.rules.registry/known-rule-p.html">known-rule?</a></li>
<li><a href="proofread.rules.registry/node-diagnostic.html">node-diagnostic</a></li>
<li><a href="proofread.rules.registry/rule-ids.html">rule-ids</a></li>
<li><a href="proofread.rules.registry/scoped.html">scoped</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.registry</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.registry.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.registry

| Symbol                                                             | Summary                                                     |
| ------------------------------------------------------------------ | ----------------------------------------------------------- |
| [`check`](proofread.rules.registry/check.html)                     | Find diagnostics for a parsed AST root.                     |
| [`check-node`](proofread.rules.registry/check-node.html)           | Find diagnostics beneath one AST node.                      |
| [`descriptors`](proofread.rules.registry/descriptors.html)         | Build proofread rule descriptors for a category.            |
| [`known-rule?`](proofread.rules.registry/known-rule-p.html)        | Test whether a rule ID is registered.                       |
| [`node-diagnostic`](proofread.rules.registry/node-diagnostic.html) | Return the first diagnostic from matching rule descriptors. |
| [`rule-ids`](proofread.rules.registry/rule-ids.html)               | Return rule IDs from descriptors.                           |
| [`scoped`](proofread.rules.registry/scoped.html)                   | Return rule descriptors for one scope.                      |

</main>
</div>
