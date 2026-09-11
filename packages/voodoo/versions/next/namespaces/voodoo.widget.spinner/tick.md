---
layout: reference
title: voodoo.widget.spinner/tick
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<ol>
<li><a href="make.html">make</a></li>
<li><a href="stock-shapes.html">stock-shapes</a></li>
<li><a class="active" href="tick.html">tick</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">tick</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget.spinner/tick.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>/</div>
<h1><code>tick</code></h1>
</header>

Advance a spinner widget by one frame.

<hr class="signature-divider">

```roo
(tick spinner)
```

<hr class="signature-divider">

## Signature:

```roo
(tick spinner)
```

| Arg     | Description                                                  |
| ------- | ------------------------------------------------------------ |
| spinner | Spinner widget returned by `make` or an earlier `tick` call. |

### Returns:

A new spinner widget at the next frame, wrapping to the first frame after the
last. The supplied widget is unchanged.

</main>
</div>
