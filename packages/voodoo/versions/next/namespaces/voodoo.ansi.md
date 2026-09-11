---
layout: reference
title: voodoo.ansi
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.ansi.html">voodoo.ansi</a>
<ol>
<li><a href="voodoo.ansi/esc.html">ESC</a></li>
<li><a href="voodoo.ansi/clear-line.html">clear-line</a></li>
<li><a href="voodoo.ansi/move-down.html">move-down</a></li>
<li><a href="voodoo.ansi/move-up.html">move-up</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.ansi</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.ansi.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.ansi

| Symbol                                      | Summary                                                                                   |
| ------------------------------------------- | ----------------------------------------------------------------------------------------- |
| [`ESC`](voodoo.ansi/esc.html)               |                                                                                          |
| [`clear-line`](voodoo.ansi/clear-line.html) | Return the ANSI sequence that erases the entire current row without<br>moving the cursor. |
| [`move-down`](voodoo.ansi/move-down.html)   | Return the ANSI sequence to move the cursor down N rows.                                  |
| [`move-up`](voodoo.ansi/move-up.html)       | Return the ANSI sequence to move the cursor up n rows.                                    |

</main>
</div>
