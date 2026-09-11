---
layout: reference
title: voodoo.editor.presentation/styles-at
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a href="decorations.html">decorations</a></li>
<li><a href="render.html">render</a></li>
<li><a href="render-through.html">render-through</a></li>
<li><a href="styled-ranges.html">styled-ranges</a></li>
<li><a class="active" href="styles-at.html">styles-at</a></li>
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
<span aria-current="page">styles-at</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.presentation/styles-at.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.presentation.html">voodoo.editor.presentation</a>/</div>
<h1><code>styles-at</code></h1>
</header>

Return the ordered visual styles applying at scalar POSITION.

<hr class="signature-divider">

```roo
(styles-at ranges position)
```

<hr class="signature-divider">

## Signature:

```roo
(styles-at ranges position)
```

| Arg      | Description                                             |
| -------- | ------------------------------------------------------- |
| ranges   | Concrete styled range maps returned by `styled-ranges`. |
| position | Unicode-scalar offset within the logical value.         |

### Returns:

A vector containing every style from ranges covering POSITION. Range and
style order are preserved so later styling layers remain visually later.

</main>
</div>
