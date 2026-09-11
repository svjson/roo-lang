---
layout: reference
title: spool.report/append-context-lines!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.report.html">spool.report</a>
<ol>
<li><a class="active" href="append-context-lines-bang.html">append-context-lines!</a></li>
<li><a href="lines.html">lines</a></li>
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
<span aria-current="page">append-context-lines!</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool.report/append-context-lines-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.report.html">spool.report</a>/</div>
<h1><code>append-context-lines!</code></h1>
</header>

Append rendered context lines depth-first.

<hr class="signature-divider">

```roo
(append-context-lines! lines contexts prefix)
```

<hr class="signature-divider">

## Signature:

```roo
(append-context-lines! lines contexts prefix)
```

| Arg      | Description                                      |
| -------- | ------------------------------------------------ |
| lines    | Mutable output line vector.                      |
| contexts | Context nodes at the current tree level.         |
| prefix   | Tree indentation inherited from parent contexts. |

### Returns:

`nil` after appending the context lines.

</main>
</div>
