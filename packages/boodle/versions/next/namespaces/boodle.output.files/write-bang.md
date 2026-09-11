---
layout: reference
title: boodle.output.files/write!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.output.files.html">boodle.output.files</a>
<ol>
<li><a class="active" href="write-bang.html">write!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.output.files.html">boodle.output.files</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">write!</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.output.files/write-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.output.files.html">boodle.output.files</a>/</div>
<h1><code>write!</code></h1>
</header>

Write generated documentation files under OUTPUT-DIR.

<hr class="signature-divider">

```roo
(write! output-dir files)
```

<hr class="signature-divider">

## Signature:

```roo
(write! output-dir files)
```

| Arg        | Description                                                |
| ---------- | ---------------------------------------------------------- |
| output-dir | Directory where generated files should be written.         |
| files      | File maps containing relative `:path` and text `:content`. |

### Returns:

The written file maps.

</main>
</div>
