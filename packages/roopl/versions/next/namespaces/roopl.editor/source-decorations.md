---
layout: reference
title: roopl.editor/source-decorations
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.editor.html">roopl.editor</a>
<ol>
<li><a href="default.html">default</a></li>
<li><a href="matching-delimiter-styling.html">matching-delimiter-styling</a></li>
<li><a class="active" href="source-decorations.html">source-decorations</a></li>
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
<span aria-current="page">source-decorations</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.editor/source-decorations.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.editor.html">roopl.editor</a>/</div>
<h1><code>source-decorations</code></h1>
</header>

Describe Roo syntax and the matched delimiter pair for BUFFER.

<hr class="signature-divider">

```roo
(source-decorations buffer)
```

<hr class="signature-divider">

## Signature:

```roo
(source-decorations buffer)
```

| Arg    | Description                                                    |
| ------ | -------------------------------------------------------------- |
| buffer | Voodoo text buffer containing Roo source and a logical cursor. |

### Returns:

Wraparoo's neutral syntax ranges followed by one
`:roopl/matching-delimiters` range when a complete pair touches the cursor.

</main>
</div>
