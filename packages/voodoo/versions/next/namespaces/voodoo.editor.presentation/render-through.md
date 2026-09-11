---
layout: reference
title: voodoo.editor.presentation/render-through
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a href="decorations.html">decorations</a></li>
<li><a href="render.html">render</a></li>
<li><a class="active" href="render-through.html">render-through</a></li>
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
<span aria-current="page">render-through</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.presentation/render-through.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>/</div>
<h1><code>render-through</code></h1>
</header>

Render SCALARS from the beginning through exclusive END.

<hr class="signature-divider">

```roo
(render-through scalars end ranges paint)
```

<hr class="signature-divider">

## Signature:

```roo
(render-through scalars end ranges paint)
```

| Arg     | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| scalars | Complete logical value split into Unicode scalar strings.         |
| end     | Exclusive scalar position to render through.                      |
| ranges  | Concrete styled ranges returned by `styled-ranges`.               |
| paint   | Optional function receiving an ordered style vector and text run. |

### Returns:

Rendered text through END. Adjacent scalars with identical styles are painted
together, and plain runs remain unchanged when PAINT is nil.

</main>
</div>
