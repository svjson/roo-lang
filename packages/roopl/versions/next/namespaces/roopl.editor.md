---
layout: reference
title: roopl.editor
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roopl.editor.html">roopl.editor</a>
<ol>
<li><a href="roopl.editor/default.html">default</a></li>
<li><a href="roopl.editor/matching-delimiter-styling.html">matching-delimiter-styling</a></li>
<li><a href="roopl.editor/source-decorations.html">source-decorations</a></li>
<li><a href="roopl.editor/syntax-styling.html">syntax-styling</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roopl.editor</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roopl.editor.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roopl.editor

| Symbol                                                                       | Summary                                                               |
| ---------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`default`](roopl.editor/default.html)                                       | Default Roopl editor with syntax presentation and matched delimiters. |
| [`matching-delimiter-styling`](roopl.editor/matching-delimiter-styling.html) | Style the boundaries of matching-delimiter decorations.               |
| [`source-decorations`](roopl.editor/source-decorations.html)                 | Describe Roo syntax and the matched delimiter pair for BUFFER.        |
| [`syntax-styling`](roopl.editor/syntax-styling.html)                         | Map neutral Roo syntax decorations to Roopl's terminal palette.       |

</main>
</div>
