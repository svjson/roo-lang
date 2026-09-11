---
layout: reference
title: moordown.render/longest-backtick-run
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../moordown.render.html">moordown.render</a>
<ol>
<li><a href="backtick-run-length.html">backtick-run-length</a></li>
<li><a href="code-span.html">code-span</a></li>
<li><a href="escaped-table-cell.html">escaped-table-cell</a></li>
<li><a href="initial-widths.html">initial-widths</a></li>
<li><a class="active" href="longest-backtick-run.html">longest-backtick-run</a></li>
<li><a href="normalized-row.html">normalized-row</a></li>
<li><a href="normalized-rows.html">normalized-rows</a></li>
<li><a href="padded-cells.html">padded-cells</a></li>
<li><a href="render-block.html">render-block</a></li>
<li><a href="render-document.html">render-document</a></li>
<li><a href="render-inline.html">render-inline</a></li>
<li><a href="render-table.html">render-table</a></li>
<li><a href="render-table-row.html">render-table-row</a></li>
<li><a href="separator-cells.html">separator-cells</a></li>
<li><a href="table-cell.html">table-cell</a></li>
<li><a href="table-widths.html">table-widths</a></li>
<li><a href="widen-column.html">widen-column</a></li>
<li><a href="widen-row.html">widen-row</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">moordown next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../moordown.render.html">moordown.render</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">longest-backtick-run</span>
</nav>
<div class="version-context">
<span class="version-package">moordown</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["moordown"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/moordown.render/longest-backtick-run.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../moordown.render.html">moordown.render</a>/</div>
<h1><code>longest-backtick-run</code></h1>
</header>

Return the longest backtick run in TEXT.

<hr class="signature-divider">

```roo
(longest-backtick-run text)
```

<hr class="signature-divider">

## Signature:

```roo
(longest-backtick-run text)
```

| Arg  | Description  |
| ---- | ------------ |
| text | Source text. |

### Returns:

Length of the longest consecutive backtick run.

</main>
</div>
