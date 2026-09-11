---
layout: reference
title: spool.report/project-context
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.report.html">spool.report</a>
<ol>
<li><a href="append-context-lines-bang.html">append-context-lines!</a></li>
<li><a href="lines.html">lines</a></li>
<li><a class="active" href="project-context.html">project-context</a></li>
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
<span aria-current="page">project-context</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool.report/project-context.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.report.html">spool.report</a>/</div>
<h1><code>project-context</code></h1>
</header>

Apply report selection and collapsing options to one context.

<hr class="signature-divider">

```roo
(project-context context options)
```

<hr class="signature-divider">

## Signature:

```roo
(project-context context options)
```

| Arg     | Description               |
| ------- | ------------------------- |
| context | Context node to project.  |
| options | Report rendering options. |

### Returns:

The projected context, or `nil` when it is not selected.

</main>
</div>
