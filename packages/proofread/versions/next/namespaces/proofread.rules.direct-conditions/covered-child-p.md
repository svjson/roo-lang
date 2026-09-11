---
layout: reference
title: proofread.rules.direct-conditions/covered-child?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.html">proofread.rules.direct-conditions</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="check-node.html">check-node</a></li>
<li><a class="active" href="covered-child-p.html">covered-child?</a></li>
<li><a href="node-diagnostic.html">node-diagnostic</a></li>
<li><a href="rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.direct-conditions.html">proofread.rules.direct-conditions</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">covered-child?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions/covered-child-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.html">proofread.rules.direct-conditions</a>/</div>
<h1><code>covered-child?</code></h1>
</header>

Test whether a child is covered by an existing parent diagnostic.

<hr class="signature-divider">

```roo
(covered-child? active-rules parent node)
```

<hr class="signature-divider">

## Signature:

```roo
(covered-child? active-rules parent node)
```

| Arg          | Description |
| ------------ | ----------- |
| active-rules |             |
| parent       |             |
| node         |             |

</main>
</div>
