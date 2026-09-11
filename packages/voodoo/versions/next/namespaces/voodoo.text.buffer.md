---
layout: reference
title: voodoo.text.buffer
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.text.buffer.html">voodoo.text.buffer</a>
<ol>
<li><a href="voodoo.text.buffer/backspace.html">backspace</a></li>
<li><a href="voodoo.text.buffer/delete.html">delete</a></li>
<li><a href="voodoo.text.buffer/delete-to-end.html">delete-to-end</a></li>
<li><a href="voodoo.text.buffer/insert.html">insert</a></li>
<li><a href="voodoo.text.buffer/make.html">make</a></li>
<li><a href="voodoo.text.buffer/move-end.html">move-end</a></li>
<li><a href="voodoo.text.buffer/move-left.html">move-left</a></li>
<li><a href="voodoo.text.buffer/move-right.html">move-right</a></li>
<li><a href="voodoo.text.buffer/move-start.html">move-start</a></li>
<li><a href="voodoo.text.buffer/move-word-left.html">move-word-left</a></li>
<li><a href="voodoo.text.buffer/move-word-right.html">move-word-right</a></li>
<li><a href="voodoo.text.buffer/position.html">position</a></li>
<li><a href="voodoo.text.buffer/text.html">text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.text.buffer</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.text.buffer.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.text.buffer

| Symbol                                                       | Summary                                                                       |
| ------------------------------------------------------------ | ----------------------------------------------------------------------------- |
| [`backspace`](voodoo.text.buffer/backspace.html)             | Delete the Unicode scalar before BUFFER's cursor.                             |
| [`delete`](voodoo.text.buffer/delete.html)                   | Delete the Unicode scalar after BUFFER's cursor.                              |
| [`delete-to-end`](voodoo.text.buffer/delete-to-end.html)     | Delete everything after BUFFER's cursor.                                      |
| [`insert`](voodoo.text.buffer/insert.html)                   | Insert TEXT at BUFFER's cursor.                                               |
| [`make`](voodoo.text.buffer/make.html)                       | Create an editable text buffer with its cursor after TEXT.                    |
| [`move-end`](voodoo.text.buffer/move-end.html)               | Move BUFFER's cursor to the end of its text.                                  |
| [`move-left`](voodoo.text.buffer/move-left.html)             | Move BUFFER's cursor left by one Unicode scalar.                              |
| [`move-right`](voodoo.text.buffer/move-right.html)           | Move BUFFER's cursor right by one Unicode scalar.                             |
| [`move-start`](voodoo.text.buffer/move-start.html)           | Move BUFFER's cursor to the start of its text.                                |
| [`move-word-left`](voodoo.text.buffer/move-word-left.html)   | Move BUFFER's cursor to the start of the preceding whitespace-delimited word. |
| [`move-word-right`](voodoo.text.buffer/move-word-right.html) | Move BUFFER's cursor to the end of the following whitespace-delimited word.   |
| [`position`](voodoo.text.buffer/position.html)               | Return BUFFER's logical cursor position.                                      |
| [`text`](voodoo.text.buffer/text.html)                       | Return BUFFER's complete text.                                                |

</main>
</div>
