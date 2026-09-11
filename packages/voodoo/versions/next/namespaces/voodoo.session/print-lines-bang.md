---
layout: reference
title: voodoo.session/print-lines!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.html">voodoo.session</a>
<ol>
<li><a href="add-bang.html">add!</a></li>
<li><a href="clear-rows-upward.html">clear-rows-upward</a></li>
<li><a href="cursor-baseline-distance.html">cursor-baseline-distance</a></li>
<li><a href="dispatch-bang.html">dispatch!</a></li>
<li><a href="evict-bang.html">evict!</a></li>
<li><a href="focus-bang.html">focus!</a></li>
<li><a href="make.html">make</a></li>
<li><a class="active" href="print-lines-bang.html">print-lines!</a></li>
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
<span aria-current="page">print-lines!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/print-lines-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>print-lines!</code></h1>
</header>

Print TEXT one physical row at a time, each preceded by `\r` and a
clear-line escape so it correctly overwrites whatever was on that row
before - necessary because TEXT may span multiple lines (see
`voodoo.widget/line-count`), and clearing only the first row would
leave stale trailing content behind on the rest when the new text is
shorter than what it's replacing.

<hr class="signature-divider">

```roo
(print-lines! prefix text)
```

<hr class="signature-divider">

PREFIX (typically a move-up escape sequence) is attached to the first
row only, since every `prn!` call carries its own trailing newline - a
bare cursor-move `prn!` call with no content would be undone by that
newline, so any positioning move must always be attached to real
content instead of sent on its own.

<hr class="signature-divider">

## Signature:

```roo
(print-lines! prefix text)
```

| Arg    | Description |
| ------ | ----------- |
| prefix |             |
| text   |             |

</main>
</div>
