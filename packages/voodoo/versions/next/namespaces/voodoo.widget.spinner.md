---
layout: reference
title: voodoo.widget.spinner
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<ol>
<li><a href="voodoo.widget.spinner/make.html">make</a></li>
<li><a href="voodoo.widget.spinner/stock-shapes.html">stock-shapes</a></li>
<li><a href="voodoo.widget.spinner/tick.html">tick</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.widget.spinner</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.widget.spinner.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.widget.spinner

| Symbol                                                    | Summary                                                                   |
| --------------------------------------------------------- | ------------------------------------------------------------------------- |
| [`make`](voodoo.widget.spinner/make.html)                 | Create a spinner widget.                                                  |
| [`stock-shapes`](voodoo.widget.spinner/stock-shapes.html) | Named spinner shapes supplied by Voodoo, as keyword-to-frame-vector data. |
| [`tick`](voodoo.widget.spinner/tick.html)                 | Advance a spinner widget by one frame.                                    |

</main>
</div>
