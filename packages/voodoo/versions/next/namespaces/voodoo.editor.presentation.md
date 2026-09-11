---
layout: reference
title: voodoo.editor.presentation
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.editor.presentation.html">voodoo.editor.presentation</a>
<ol>
<li><a href="voodoo.editor.presentation/decorations.html">decorations</a></li>
<li><a href="voodoo.editor.presentation/render.html">render</a></li>
<li><a href="voodoo.editor.presentation/render-through.html">render-through</a></li>
<li><a href="voodoo.editor.presentation/styled-ranges.html">styled-ranges</a></li>
<li><a href="voodoo.editor.presentation/styles-at.html">styles-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.editor.presentation</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.editor.presentation.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.editor.presentation

| Symbol                                                             | Summary                                                             |
| ------------------------------------------------------------------ | ------------------------------------------------------------------- |
| [`decorations`](voodoo.editor.presentation/decorations.html)       | Collect semantic decorations contributed by EDITOR for BUFFER.      |
| [`render`](voodoo.editor.presentation/render.html)                 | Render BUFFER's logical value through EDITOR's presentation layers. |
| [`render-through`](voodoo.editor.presentation/render-through.html) | Render SCALARS from the beginning through exclusive END.            |
| [`styled-ranges`](voodoo.editor.presentation/styled-ranges.html)   | Collect concrete styled ranges contributed by EDITOR for CONTEXT.   |
| [`styles-at`](voodoo.editor.presentation/styles-at.html)           | Return the ordered visual styles applying at scalar POSITION.       |

</main>
</div>
