---
layout: reference
title: proofread.rules.registry/scoped
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.registry.html">proofread.rules.registry</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="check-node.html">check-node</a></li>
<li><a href="descriptors.html">descriptors</a></li>
<li><a href="known-rule-p.html">known-rule?</a></li>
<li><a href="node-diagnostic.html">node-diagnostic</a></li>
<li><a href="rule-ids.html">rule-ids</a></li>
<li><a class="active" href="scoped.html">scoped</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.registry.html">proofread.rules.registry</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">scoped</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.registry/scoped.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.registry.html">proofread.rules.registry</a>/</div>
<h1><code>scoped</code></h1>
</header>

Return rule descriptors for one scope.

<hr class="signature-divider">

```roo
(scoped rules scope)
```

<hr class="signature-divider">

## Signature:

```roo
(scoped rules scope)
```

| Arg   | Description           |
| ----- | --------------------- |
| rules | Rule descriptor maps. |
| scope | Scope keyword.        |

### Returns:

Rule descriptors whose `:scope` is `scope`.

</main>
</div>
