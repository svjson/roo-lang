---
layout: reference
title: voodoo.widget.spinner/stock-shapes
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<ol>
<li><a href="make.html">make</a></li>
<li><a class="active" href="stock-shapes.html">stock-shapes</a></li>
<li><a href="tick.html">tick</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">stock-shapes</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget.spinner/stock-shapes.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>/</div>
<h1><code>stock-shapes</code></h1>
</header>

Named spinner shapes supplied by Voodoo, as keyword-to-frame-vector data.



`:dots` is the default clockwise Braille shape. `:line` cycles through the
four ASCII line orientations.

```roo
(:line stock-shapes)
;; => ["/" "-" "\\" "|"]
```



</main>
</div>
