---
layout: reference
title: voodoo.editor
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.editor.html">voodoo.editor</a>
<ol>
<li><a href="voodoo.editor/compose.html">compose</a></li>
<li><a href="voodoo.editor/dispatch.html">dispatch</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.editor</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.editor.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.editor

| Symbol                                    | Summary                                                                  |
| ----------------------------------------- | ------------------------------------------------------------------------ |
| [`compose`](voodoo.editor/compose.html)   | Compose EDITORS from general behavior to increasingly specific overlays. |
| [`dispatch`](voodoo.editor/dispatch.html) | Dispatch EVENT through EDITOR against BUFFER.                            |

</main>
</div>
