---
layout: reference
title: voodoo.session/cursor-baseline-distance
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.html">voodoo.session</a>
<ol>
<li><a href="add-bang.html">add!</a></li>
<li><a href="clear-rows-upward.html">clear-rows-upward</a></li>
<li><a class="active" href="cursor-baseline-distance.html">cursor-baseline-distance</a></li>
<li><a href="dispatch-bang.html">dispatch!</a></li>
<li><a href="evict-bang.html">evict!</a></li>
<li><a href="focus-bang.html">focus!</a></li>
<li><a href="make.html">make</a></li>
<li><a href="print-lines-bang.html">print-lines!</a></li>
<li><a href="redraw-bang.html">redraw!</a></li>
<li><a href="remove-bang.html">remove!</a></li>
<li><a href="reset-terminal-cursor-bang.html">reset-terminal-cursor!</a></li>
<li><a href="restore-terminal-cursor-bang.html">restore-terminal-cursor!</a></li>
<li><a href="total-height.html">total-height</a></li>
<li><a href="update-bang.html">update!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.session.html">voodoo.session</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cursor-baseline-distance</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/cursor-baseline-distance.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>cursor-baseline-distance</code></h1>
</header>

Return the rows between a tracked widget cursor and the render baseline.

<hr class="signature-divider">

```roo
(cursor-baseline-distance session handle cursor)
```

<hr class="signature-divider">

SESSION's render baseline is the row immediately below its last widget.
HANDLE identifies the widget containing the zero-based CURSOR row.

<hr class="signature-divider">

## Signature:

```roo
(cursor-baseline-distance session handle cursor)
```

| Arg     | Description |
| ------- | ----------- |
| session |             |
| handle  |             |
| cursor  |             |

</main>
</div>
