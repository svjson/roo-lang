---
layout: reference
title: voodoo.text.buffer/position
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.text.buffer.html">voodoo.text.buffer</a>
<ol>
<li><a href="backspace.html">backspace</a></li>
<li><a href="delete.html">delete</a></li>
<li><a href="delete-to-end.html">delete-to-end</a></li>
<li><a href="insert.html">insert</a></li>
<li><a href="make.html">make</a></li>
<li><a href="move-end.html">move-end</a></li>
<li><a href="move-left.html">move-left</a></li>
<li><a href="move-right.html">move-right</a></li>
<li><a href="move-start.html">move-start</a></li>
<li><a href="move-word-left.html">move-word-left</a></li>
<li><a href="move-word-right.html">move-word-right</a></li>
<li><a class="active" href="position.html">position</a></li>
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
<span aria-current="page">position</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.text.buffer/position.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.text.buffer.html">voodoo.text.buffer</a>/</div>
<h1><code>position</code></h1>
</header>

Return BUFFER's logical cursor position.

<hr class="signature-divider">

```roo
(position buffer)
```

<hr class="signature-divider">

## Signature:

```roo
(position buffer)
```

| Arg    | Description                                                       |
| ------ | ----------------------------------------------------------------- |
| buffer | Buffer returned by `make` or another operation in this namespace. |

### Returns:

The number of Unicode scalars preceding the cursor.

</main>
</div>
