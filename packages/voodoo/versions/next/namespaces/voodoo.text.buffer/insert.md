---
layout: reference
title: voodoo.text.buffer/insert
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.text.buffer.html">voodoo.text.buffer</a>
<ol>
<li><a href="backspace.html">backspace</a></li>
<li><a href="delete.html">delete</a></li>
<li><a href="delete-to-end.html">delete-to-end</a></li>
<li><a class="active" href="insert.html">insert</a></li>
<li><a href="make.html">make</a></li>
<li><a href="move-end.html">move-end</a></li>
<li><a href="move-left.html">move-left</a></li>
<li><a href="move-right.html">move-right</a></li>
<li><a href="move-start.html">move-start</a></li>
<li><a href="move-word-left.html">move-word-left</a></li>
<li><a href="move-word-right.html">move-word-right</a></li>
<li><a href="position.html">position</a></li>
<li><a href="text.html">text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.text.buffer.html">voodoo.text.buffer</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">insert</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.text.buffer/insert.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.text.buffer.html">voodoo.text.buffer</a>/</div>
<h1><code>insert</code></h1>
</header>

Insert TEXT at BUFFER's cursor.

<hr class="signature-divider">

```roo
(insert buffer text)
```

<hr class="signature-divider">

## Signature:

```roo
(insert buffer text)
```

| Arg    | Description                                                               |
| ------ | ------------------------------------------------------------------------- |
| buffer | Buffer to edit.                                                           |
| text   | String to insert. It may contain multiple Unicode scalars or<br>newlines. |

### Returns:

A buffer with TEXT inserted and the cursor immediately after it.

</main>
</div>
