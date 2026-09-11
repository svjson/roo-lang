---
layout: reference
title: voodoo.widget.spinner/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>
<ol>
<li><a class="active" href="make.html">make</a></li>
<li><a href="stock-shapes.html">stock-shapes</a></li>
<li><a href="tick.html">tick</a></li>
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
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget.spinner/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.spinner.html">voodoo.widget.spinner</a>/</div>
<h1><code>make</code></h1>
</header>

Create a spinner widget.

<hr class="signature-divider">

```roo
(make opts)
```

<hr class="signature-divider">

## Signature:

```roo
(make opts)
```

| Arg  | Description                                                                                                                                                                                                                                                                                                                 |
| ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| opts | Widget option map. `:label` is rendered before the current frame.<br>`:shape` selects a key from `stock-shapes` and defaults to `:dots`.<br>`:frames` may instead supply a custom nonempty vector of frame strings.<br>`:frame-index` selects the initial frame and defaults to zero. Other widget<br>fields are preserved. |

### Returns:

A spinner widget whose `:text` contains its label and current frame. Frame
indices wrap around the selected frame vector. The widget defaults to
`voodoo.widget/tty-only` degradation.

```roo
(spinner/make {:label "src/app.roo" :shape :line})
```

</main>
</div>
