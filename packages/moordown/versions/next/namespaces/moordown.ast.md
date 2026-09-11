---
layout: reference
title: moordown.ast
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="moordown.ast.html">moordown.ast</a>
<ol>
<li><a href="moordown.ast/code.html">code</a></li>
<li><a href="moordown.ast/code-block.html">code-block</a></li>
<li><a href="moordown.ast/document.html">document</a></li>
<li><a href="moordown.ast/heading.html">heading</a></li>
<li><a href="moordown.ast/link.html">link</a></li>
<li><a href="moordown.ast/markdown-text.html">markdown-text</a></li>
<li><a href="moordown.ast/paragraph.html">paragraph</a></li>
<li><a href="moordown.ast/table.html">table</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">moordown next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">moordown.ast</span>
</nav>
<div class="version-context">
<span class="version-package">moordown</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["moordown"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/moordown.ast.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# moordown.ast

| Symbol                                             | Summary                                   |
| -------------------------------------------------- | ----------------------------------------- |
| [`code`](moordown.ast/code.html)                   | Create an inline Markdown code node.      |
| [`code-block`](moordown.ast/code-block.html)       | Create a fenced Markdown code block node. |
| [`document`](moordown.ast/document.html)           | Create a Markdown document node.          |
| [`heading`](moordown.ast/heading.html)             | Create a Markdown heading node.           |
| [`link`](moordown.ast/link.html)                   | Create a Markdown link inline node.       |
| [`markdown-text`](moordown.ast/markdown-text.html) | Normalize a value to Markdown text.       |
| [`paragraph`](moordown.ast/paragraph.html)         | Create a Markdown paragraph node.         |
| [`table`](moordown.ast/table.html)                 | Create a Markdown table node.             |

</main>
</div>
