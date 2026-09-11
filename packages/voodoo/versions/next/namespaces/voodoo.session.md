---
layout: reference
title: voodoo.session
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.session.html">voodoo.session</a>
<ol>
<li><a href="voodoo.session/add-bang.html">add!</a></li>
<li><a href="voodoo.session/clear-rows-upward.html">clear-rows-upward</a></li>
<li><a href="voodoo.session/cursor-baseline-distance.html">cursor-baseline-distance</a></li>
<li><a href="voodoo.session/dispatch-bang.html">dispatch!</a></li>
<li><a href="voodoo.session/evict-bang.html">evict!</a></li>
<li><a href="voodoo.session/focus-bang.html">focus!</a></li>
<li><a href="voodoo.session/make.html">make</a></li>
<li><a href="voodoo.session/print-lines-bang.html">print-lines!</a></li>
<li><a href="voodoo.session/redraw-bang.html">redraw!</a></li>
<li><a href="voodoo.session/remove-bang.html">remove!</a></li>
<li><a href="voodoo.session/reset-terminal-cursor-bang.html">reset-terminal-cursor!</a></li>
<li><a href="voodoo.session/restore-terminal-cursor-bang.html">restore-terminal-cursor!</a></li>
<li><a href="voodoo.session/total-height.html">total-height</a></li>
<li><a href="voodoo.session/update-bang.html">update!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.session</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.session.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.session

| Symbol                                                                         | Summary                                                                                                                                                                                                                                                                                                                                                                                        |
| ------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`add!`](voodoo.session/add-bang.html)                                         | Register a new widget in a session and render it immediately.                                                                                                                                                                                                                                                                                                                                  |
| [`clear-rows-upward`](voodoo.session/clear-rows-upward.html)                   | Return an escape sequence that clears N rows, moving up one row at a<br>time from the cursor's current position, ending at the top of that<br>block. Built from repeated move-up + clear-line since there's no<br>move-down primitive - none is needed here, since erasing bottom-to-top<br>only ever moves in one direction.                                                                  |
| [`cursor-baseline-distance`](voodoo.session/cursor-baseline-distance.html)     | Return the rows between a tracked widget cursor and the render baseline.                                                                                                                                                                                                                                                                                                                       |
| [`dispatch!`](voodoo.session/dispatch-bang.html)                               | Dispatch EVENT to the focused widget and apply its event result.                                                                                                                                                                                                                                                                                                                               |
| [`evict!`](voodoo.session/evict-bang.html)                                     | Drop a widget from session bookkeeping without altering rendered content.                                                                                                                                                                                                                                                                                                                      |
| [`focus!`](voodoo.session/focus-bang.html)                                     | Give keyboard focus to a tracked widget.                                                                                                                                                                                                                                                                                                                                                       |
| [`make`](voodoo.session/make.html)                                             | Create a new voodoo session.                                                                                                                                                                                                                                                                                                                                                                   |
| [`print-lines!`](voodoo.session/print-lines-bang.html)                         | Print TEXT one physical row at a time, each preceded by `\r` and a<br>clear-line escape so it correctly overwrites whatever was on that row<br>before - necessary because TEXT may span multiple lines (see<br>`voodoo.widget/line-count`), and clearing only the first row would<br>leave stale trailing content behind on the rest when the new text is<br>shorter than what it's replacing. |
| [`redraw!`](voodoo.session/redraw-bang.html)                                   | Rewrite widgets one at a time, starting at the current cursor row.                                                                                                                                                                                                                                                                                                                             |
| [`remove!`](voodoo.session/remove-bang.html)                                   | Remove a widget from a session, erasing it from the terminal.                                                                                                                                                                                                                                                                                                                                  |
| [`reset-terminal-cursor!`](voodoo.session/reset-terminal-cursor-bang.html)     | Move a focused widget cursor back to SESSION's render baseline.                                                                                                                                                                                                                                                                                                                                |
| [`restore-terminal-cursor!`](voodoo.session/restore-terminal-cursor-bang.html) | Place the terminal cursor at SESSION's focused widget cursor.                                                                                                                                                                                                                                                                                                                                  |
| [`total-height`](voodoo.session/total-height.html)                             |                                                                                                                                                                                                                                                                                                                                                                                                |
| [`update!`](voodoo.session/update-bang.html)                                   | Replace a widget's spec and re-render it in place.                                                                                                                                                                                                                                                                                                                                             |

</main>
</div>
