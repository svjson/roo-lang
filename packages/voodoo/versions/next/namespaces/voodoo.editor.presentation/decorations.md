---
layout: reference
title: voodoo.editor.presentation/decorations
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a class="active" href="decorations.html">decorations</a></li>
<li><a href="render.html">render</a></li>
<li><a href="render-through.html">render-through</a></li>
<li><a href="styled-ranges.html">styled-ranges</a></li>
<li><a href="styles-at.html">styles-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">decorations</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.presentation/decorations.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>/</div>
<h1><code>decorations</code></h1>
</header>

Collect semantic decorations contributed by EDITOR for BUFFER.

<hr class="signature-divider">

```roo
(decorations editor buffer)
```

<hr class="signature-divider">

## Signature:

```roo
(decorations editor buffer)
```

| Arg    | Description                                                           |
| ------ | --------------------------------------------------------------------- |
| editor | Composed editor whose layers may contain `:decorations`<br>functions. |
| buffer | Current editable text buffer.                                         |

### Returns:

A vector of typed range maps in base-to-overlay editor-layer order. A layer's
`:decorations` function receives BUFFER and may return zero or more maps such
as `{:range {:start 0 :end 5} :role :application/selection}`.

</main>
</div>
