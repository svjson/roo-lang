---
layout: reference
title: proofread.rules.diagnostics/rewrite
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>
<ol>
<li><a href="edit.html">edit</a></li>
<li><a href="make.html">make</a></li>
<li><a href="make-file.html">make-file</a></li>
<li><a href="make-suggesting.html">make-suggesting</a></li>
<li><a class="active" href="rewrite.html">rewrite</a></li>
<li><a href="with-suggestions.html">with-suggestions</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">rewrite</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.diagnostics/rewrite.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>/</div>
<h1><code>rewrite</code></h1>
</header>

Build a single-edit rewrite suggestion.

<hr class="signature-divider">

```roo
(rewrite node title replacement)
```

<hr class="signature-divider">

## Signature:

```roo
(rewrite node title replacement)
```

| Arg         | Description                   |
| ----------- | ----------------------------- |
| node        | A `roo.ast` node map.         |
| title       | User-facing suggestion title. |
| replacement | Replacement source text.      |

### Returns:

A suggestion map containing replacement edits.

</main>
</div>
