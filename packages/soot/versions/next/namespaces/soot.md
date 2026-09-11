---
layout: reference
title: soot
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="soot.html">soot</a>
<ol>
<li><a href="soot/bg.html">bg</a></li>
<li><a href="soot/bg-black.html">bg-black</a></li>
<li><a href="soot/bg-blue.html">bg-blue</a></li>
<li><a href="soot/bg-cyan.html">bg-cyan</a></li>
<li><a href="soot/bg-gray.html">bg-gray</a></li>
<li><a href="soot/bg-green.html">bg-green</a></li>
<li><a href="soot/bg-grey.html">bg-grey</a></li>
<li><a href="soot/bg-magenta.html">bg-magenta</a></li>
<li><a href="soot/bg-red.html">bg-red</a></li>
<li><a href="soot/bg-rgb.html">bg-rgb</a></li>
<li><a href="soot/bg-white.html">bg-white</a></li>
<li><a href="soot/bg-yellow.html">bg-yellow</a></li>
<li><a href="soot/black.html">black</a></li>
<li><a href="soot/blue.html">blue</a></li>
<li><a href="soot/bold.html">bold</a></li>
<li><a href="soot/close-sequence.html">close-sequence</a></li>
<li><a href="soot/code-for.html">code-for</a></li>
<li><a href="soot/custom-style.html">custom-style</a></li>
<li><a href="soot/cyan.html">cyan</a></li>
<li><a href="soot/dim.html">dim</a></li>
<li><a href="soot/esc.html">esc</a></li>
<li><a href="soot/fg.html">fg</a></li>
<li><a href="soot/gray.html">gray</a></li>
<li><a href="soot/green.html">green</a></li>
<li><a href="soot/grey.html">grey</a></li>
<li><a href="soot/hidden.html">hidden</a></li>
<li><a href="soot/inverse.html">inverse</a></li>
<li><a href="soot/italic.html">italic</a></li>
<li><a href="soot/known-styles.html">known-styles</a></li>
<li><a href="soot/magenta.html">magenta</a></li>
<li><a href="soot/normalize-styles.html">normalize-styles</a></li>
<li><a href="soot/open-sequence.html">open-sequence</a></li>
<li><a href="soot/paint.html">paint</a></li>
<li><a href="soot/print-bang.html">print!</a></li>
<li><a href="soot/red.html">red</a></li>
<li><a href="soot/resolve-style.html">resolve-style</a></li>
<li><a href="soot/rgb.html">rgb</a></li>
<li><a href="soot/sgr.html">sgr</a></li>
<li><a href="soot/strikethrough.html">strikethrough</a></li>
<li><a href="soot/underline.html">underline</a></li>
<li><a href="soot/white.html">white</a></li>
<li><a href="soot/yellow.html">yellow</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">soot next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">soot</span>
</nav>
<div class="version-context">
<span class="version-package">soot</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["soot"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/soot.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# soot

| Symbol                                           | Summary                                                               |
| ------------------------------------------------ | --------------------------------------------------------------------- |
| [`bg`](soot/bg.html)                             |                                                                       |
| [`bg-black`](soot/bg-black.html)                 |                                                                       |
| [`bg-blue`](soot/bg-blue.html)                   |                                                                       |
| [`bg-cyan`](soot/bg-cyan.html)                   |                                                                       |
| [`bg-gray`](soot/bg-gray.html)                   |                                                                       |
| [`bg-green`](soot/bg-green.html)                 |                                                                       |
| [`bg-grey`](soot/bg-grey.html)                   |                                                                       |
| [`bg-magenta`](soot/bg-magenta.html)             |                                                                       |
| [`bg-red`](soot/bg-red.html)                     |                                                                       |
| [`bg-rgb`](soot/bg-rgb.html)                     |                                                                       |
| [`bg-white`](soot/bg-white.html)                 |                                                                       |
| [`bg-yellow`](soot/bg-yellow.html)               |                                                                       |
| [`black`](soot/black.html)                       |                                                                       |
| [`blue`](soot/blue.html)                         |                                                                       |
| [`bold`](soot/bold.html)                         |                                                                       |
| [`close-sequence`](soot/close-sequence.html)     |                                                                       |
| [`code-for`](soot/code-for.html)                 |                                                                       |
| [`custom-style`](soot/custom-style.html)         | Create a style map from explicit SGR open and close code strings.     |
| [`cyan`](soot/cyan.html)                         |                                                                       |
| [`dim`](soot/dim.html)                           |                                                                       |
| [`esc`](soot/esc.html)                           |                                                                      |
| [`fg`](soot/fg.html)                             |                                                                       |
| [`gray`](soot/gray.html)                         |                                                                       |
| [`green`](soot/green.html)                       |                                                                       |
| [`grey`](soot/grey.html)                         |                                                                       |
| [`hidden`](soot/hidden.html)                     |                                                                       |
| [`inverse`](soot/inverse.html)                   |                                                                       |
| [`italic`](soot/italic.html)                     |                                                                       |
| [`known-styles`](soot/known-styles.html)         |                                                                       |
| [`magenta`](soot/magenta.html)                   |                                                                       |
| [`normalize-styles`](soot/normalize-styles.html) |                                                                       |
| [`open-sequence`](soot/open-sequence.html)       |                                                                       |
| [`paint`](soot/paint.html)                       | Apply one style, a vector of styles, or custom style maps to a value. |
| [`print!`](soot/print-bang.html)                 | Print a styled value and return the original value.                   |
| [`red`](soot/red.html)                           |                                                                       |
| [`resolve-style`](soot/resolve-style.html)       |                                                                       |
| [`rgb`](soot/rgb.html)                           |                                                                       |
| [`sgr`](soot/sgr.html)                           | Return an ANSI SGR sequence for one or more code strings.             |
| [`strikethrough`](soot/strikethrough.html)       |                                                                       |
| [`underline`](soot/underline.html)               |                                                                       |
| [`white`](soot/white.html)                       |                                                                       |
| [`yellow`](soot/yellow.html)                     |                                                                       |

</main>
</div>
