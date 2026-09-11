---
layout: reference
title: roopl.history/load!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.history.html">roopl.history</a>
<ol>
<li><a href="default-path-bang.html">default-path!</a></li>
<li><a class="active" href="load-bang.html">load!</a></li>
<li><a href="max-entries.html">max-entries</a></li>
<li><a href="record-bang.html">record!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.history.html">roopl.history</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">load!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.history/load-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.history.html">roopl.history</a>/</div>
<h1><code>load!</code></h1>
</header>

Load recent nonblank commands from PATH.

<hr class="signature-divider">

```roo
(load! path)
```

<hr class="signature-divider">

## Signature:

```roo
(load! path)
```

| Arg  | Description                                                 |
| ---- | ----------------------------------------------------------- |
| path | History file path. A missing file represents empty history. |

### Returns:

A vector containing at most the newest `max-entries` commands.

</main>
</div>
