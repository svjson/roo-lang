---
layout: reference
title: voodoo.editor.presentation/styled-ranges
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a href="decorations.html">decorations</a></li>
<li><a href="render.html">render</a></li>
<li><a href="render-through.html">render-through</a></li>
<li><a class="active" href="styled-ranges.html">styled-ranges</a></li>
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
<span aria-current="page">styled-ranges</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.presentation/styled-ranges.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>/</div>
<h1><code>styled-ranges</code></h1>
</header>

Collect concrete styled ranges contributed by EDITOR for CONTEXT.

<hr class="signature-divider">

```roo
(styled-ranges editor context)
```

<hr class="signature-divider">

## Signature:

```roo
(styled-ranges editor context)
```

| Arg     | Description                                                                                           |
| ------- | ----------------------------------------------------------------------------------------------------- |
| editor  | Composed editor whose layers may contain `:styling` functions.                                        |
| context | Presentation context containing `:value`, `:cursor-position`,<br>and ordered semantic `:decorations`. |

### Returns:

A vector of maps shaped as `{:range {:start n :end n} :styles [...]}` in
base-to-overlay layer order. Styling functions receive CONTEXT and may map
one semantic decoration to zero, one, or several concrete ranges.

</main>
</div>
