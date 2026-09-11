---
layout: reference
title: spool.report/lines
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.report.html">spool.report</a>
<ol>
<li><a href="append-context-lines-bang.html">append-context-lines!</a></li>
<li><a class="active" href="lines.html">lines</a></li>
<li><a href="project-context.html">project-context</a></li>
<li><a href="project-contexts.html">project-contexts</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../spool.report.html">spool.report</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lines</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool.report/lines.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.report.html">spool.report</a>/</div>
<h1><code>lines</code></h1>
</header>

Render a report tree as text lines.

<hr class="signature-divider">

```roo
(lines tree & options)
```

<hr class="signature-divider">

## Signature:

```roo
(lines tree & options)
```

| Arg     | Description                                |
| ------- | ------------------------------------------ |
| tree    | Report tree containing `:contexts`.        |
| options | Optional selection and collapsing options. |

### Returns:

Rendered lines beginning with the `spool` heading.

</main>
</div>
