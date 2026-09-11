---
layout: reference
title: roopl.editor/default
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.editor.html">roopl.editor</a>
<ol>
<li><a class="active" href="default.html">default</a></li>
<li><a href="matching-delimiter-styling.html">matching-delimiter-styling</a></li>
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
<span aria-current="page">default</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.editor/default.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.editor.html">roopl.editor</a>/</div>
<h1><code>default</code></h1>
</header>

Default Roopl editor with syntax presentation and matched delimiters.



The value is a composed Voodoo editor. One presentation layer contributes a
neutral syntax and matching-delimiter ranges from Wraparoo. Separate styling
layers map syntax roles to Roopl's terminal palette and matched boundaries to
bold. The top-level `:paint` function is `soot/paint`.



</main>
</div>
