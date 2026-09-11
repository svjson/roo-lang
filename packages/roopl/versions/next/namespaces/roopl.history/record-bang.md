---
layout: reference
title: roopl.history/record!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.history.html">roopl.history</a>
<ol>
<li><a href="default-path-bang.html">default-path!</a></li>
<li><a href="load-bang.html">load!</a></li>
<li><a href="max-entries.html">max-entries</a></li>
<li><a class="active" href="record-bang.html">record!</a></li>
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
<span aria-current="page">record!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.history/record-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.history.html">roopl.history</a>/</div>
<h1><code>record!</code></h1>
</header>

Record COMMAND and atomically persist recent history to PATH.

<hr class="signature-divider">

```roo
(record! path entries command)
```

<hr class="signature-divider">

## Signature:

```roo
(record! path entries command)
```

| Arg     | Description                                         |
| ------- | --------------------------------------------------- |
| path    | History file path to replace.                       |
| entries | Existing command history in oldest-to-newest order. |
| command | Nonblank, single-line command to add.               |

### Returns:

A vector containing COMMAND and at most the newest `max-entries` entries.

</main>
</div>
