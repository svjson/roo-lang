---
layout: reference
title: roopl.editor/syntax-styling
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.editor.html">roopl.editor</a>
<ol>
<li><a href="default.html">default</a></li>
<li><a href="matching-delimiter-styling.html">matching-delimiter-styling</a></li>
<li><a href="source-decorations.html">source-decorations</a></li>
<li><a class="active" href="syntax-styling.html">syntax-styling</a></li>
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
<span aria-current="page">syntax-styling</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.editor/syntax-styling.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.editor.html">roopl.editor</a>/</div>
<h1><code>syntax-styling</code></h1>
</header>

Map neutral Roo syntax decorations to Roopl's terminal palette.

<hr class="signature-divider">

```roo
(syntax-styling context)
```

<hr class="signature-divider">

## Signature:

```roo
(syntax-styling context)
```

| Arg     | Description                                                                                  |
| ------- | -------------------------------------------------------------------------------------------- |
| context | Voodoo presentation context containing ordered<br>`:decorations` over an editable Roo value. |

### Returns:

Concrete styled ranges for recognized `:roo.syntax/*` roles. Unknown roles
are omitted so other application layers can style them independently.

</main>
</div>
