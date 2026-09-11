---
layout: reference
title: roopl.editor/matching-delimiter-styling
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.editor.html">roopl.editor</a>
<ol>
<li><a href="default.html">default</a></li>
<li><a class="active" href="matching-delimiter-styling.html">matching-delimiter-styling</a></li>
<li><a href="source-decorations.html">source-decorations</a></li>
<li><a href="syntax-styling.html">syntax-styling</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.editor.html">roopl.editor</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">matching-delimiter-styling</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.editor/matching-delimiter-styling.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.editor.html">roopl.editor</a>/</div>
<h1><code>matching-delimiter-styling</code></h1>
</header>

Style the boundaries of matching-delimiter decorations.

<hr class="signature-divider">

```roo
(matching-delimiter-styling context)
```

<hr class="signature-divider">

## Signature:

```roo
(matching-delimiter-styling context)
```

| Arg     | Description                                                                                  |
| ------- | -------------------------------------------------------------------------------------------- |
| context | Voodoo presentation context containing ordered<br>`:decorations` over an editable Roo value. |

### Returns:

Concrete one-scalar ranges for both boundaries of every
`:roopl/matching-delimiters` decoration. Each range carries Soot's
`:bold` style.

</main>
</div>
