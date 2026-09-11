---
layout: reference
title: voodoo.ansi/move-up
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.ansi.html">voodoo.ansi</a>
<ol>
<li><a href="esc.html">ESC</a></li>
<li><a href="clear-line.html">clear-line</a></li>
<li><a href="move-down.html">move-down</a></li>
<li><a class="active" href="move-up.html">move-up</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.ansi.html">voodoo.ansi</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">move-up</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.ansi/move-up.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.ansi.html">voodoo.ansi</a>/</div>
<h1><code>move-up</code></h1>
</header>

Return the ANSI sequence to move the cursor up n rows.

<hr class="signature-divider">

```roo
(move-up n)
```

<hr class="signature-divider">

## Signature:

```roo
(move-up n)
```

| Arg | Description                |
| --- | -------------------------- |
| n   | Number of rows to move up. |

### Returns:

The escape sequence, or "" when n is not positive - an ANSI parameter
of 0 is clamped to 1 row by the terminal, not treated as a no-op, so a
non-positive n must be filtered out here instead.

</main>
</div>
