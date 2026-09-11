---
layout: reference
title: voodoo.ansi/clear-line
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.ansi.html">voodoo.ansi</a>
<ol>
<li><a href="esc.html">ESC</a></li>
<li><a class="active" href="clear-line.html">clear-line</a></li>
<li><a href="move-down.html">move-down</a></li>
<li><a href="move-up.html">move-up</a></li>
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
<span aria-current="page">clear-line</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.ansi/clear-line.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.ansi.html">voodoo.ansi</a>/</div>
<h1><code>clear-line</code></h1>
</header>

Return the ANSI sequence that erases the entire current row without
moving the cursor.

<hr class="signature-divider">

```roo
(clear-line)
```

<hr class="signature-divider">

### Returns:

The escape sequence.

<hr class="signature-divider">

## Signature:

```roo
(clear-line)
```

### Returns:

The escape sequence.

</main>
</div>
