---
layout: reference
title: spool/enter!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.html">spool</a>
<ol>
<li><a class="active" href="enter-bang.html">enter!</a></li>
<li><a href="leave-bang.html">leave!</a></li>
<li><a href="record-bang.html">record!</a></li>
<li><a href="report-bang.html">report!</a></li>
<li><a href="reset-bang.html">reset!</a></li>
<li><a href="wrap.html">wrap</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../spool.html">spool</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">enter!</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool/enter-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.html">spool</a>/</div>
<h1><code>enter!</code></h1>
</header>

Open a timed context.

<hr class="signature-divider">

```roo
(enter! context-key & metadata &rest)
```

<hr class="signature-divider">

## Signature:

```roo
(enter! context-key & metadata &rest)
```

| Arg         | Description                                            |
| ----------- | ------------------------------------------------------ |
| context-key | Value identifying the context.                         |
| metadata    | Optional metadata map recorded when the context opens. |
| &rest       | Additional metadata maps to merge.                     |

### Returns:

The new context's path in the report tree.

</main>
</div>
