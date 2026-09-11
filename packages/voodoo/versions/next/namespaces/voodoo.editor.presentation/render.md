---
layout: reference
title: voodoo.editor.presentation/render
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a href="decorations.html">decorations</a></li>
<li><a class="active" href="render.html">render</a></li>
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
<span aria-current="page">render</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.presentation/render.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>/</div>
<h1><code>render</code></h1>
</header>

Render BUFFER's logical value through EDITOR's presentation layers.

<hr class="signature-divider">

```roo
(render editor buffer)
```

<hr class="signature-divider">

## Signature:

```roo
(render editor buffer)
```

| Arg    | Description                                                                                                                                                     |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| editor | Composed editor. Layers may contribute semantic decorations and<br>styling, while optional top-level `:paint` converts styles and text to a<br>rendered string. |
| buffer | Current editable text buffer.                                                                                                                                   |

### Returns:

`{:text rendered-value :text-before rendered-prefix}`. Both strings derive
from the same semantic decorations and concrete styled ranges, while all
positions remain Unicode-scalar offsets into the undecorated buffer value.

</main>
</div>
