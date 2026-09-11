---
layout: reference
title: workbook.value/can-undo?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../workbook.value.html">workbook.value</a>
<ol>
<li><a href="can-redo-p.html">can-redo?</a></li>
<li><a class="active" href="can-undo-p.html">can-undo?</a></li>
<li><a href="default-limit.html">default-limit</a></li>
<li><a href="dirty-p.html">dirty?</a></li>
<li><a href="history.html">history</a></li>
<li><a href="init.html">init</a></li>
<li><a href="initial-history.html">initial-history</a></li>
<li><a href="mark-saved.html">mark-saved</a></li>
<li><a href="redo-label.html">redo-label</a></li>
<li><a href="undo-label.html">undo-label</a></li>
<li><a href="value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">workbook next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../workbook.value.html">workbook.value</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">can-undo?</span>
</nav>
<div class="version-context">
<span class="version-package">workbook</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["workbook"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/workbook.value/can-undo-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../workbook.value.html">workbook.value</a>/</div>
<h1><code>can-undo?</code></h1>
</header>

<hr class="signature-divider">

```roo
(can-undo? book)
```

<hr class="signature-divider">

## Signature:

```roo
(can-undo? book)
```

| Arg  | Description |
| ---- | ----------- |
| book |             |

</main>
</div>
