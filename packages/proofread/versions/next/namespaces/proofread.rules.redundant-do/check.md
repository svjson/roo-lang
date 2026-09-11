---
layout: reference
title: proofread.rules.redundant-do/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.redundant-do.html">proofread.rules.redundant-do</a>
<ol>
<li><a class="active" href="check.html">check</a></li>
<li><a href="rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.redundant-do.html">proofread.rules.redundant-do</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.redundant-do/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.redundant-do.html">proofread.rules.redundant-do</a>/</div>
<h1><code>check</code></h1>
</header>

Find redundant `do` forms beneath one AST root.

<hr class="signature-divider">

```roo
(check active-rules root)
```

<hr class="signature-divider">

## Signature:

```roo
(check active-rules root)
```

| Arg          | Description                     |
| ------------ | ------------------------------- |
| active-rules | Rule descriptors to run.        |
| root         | A top-level `roo.ast` node map. |

### Returns:

A vector of diagnostic maps.

</main>
</div>
