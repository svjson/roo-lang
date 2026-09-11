---
layout: reference
title: lookup.command.audit/diagnostic-line
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.command.audit.html">lookup.command.audit</a>
<ol>
<li><a href="audit-index-bang.html">audit-index!</a></li>
<li><a href="command.html">command</a></li>
<li><a href="diagnostic-extra.html">diagnostic-extra</a></li>
<li><a class="active" href="diagnostic-line.html">diagnostic-line</a></li>
<li><a href="execute-bang.html">execute!</a></li>
<li><a href="text-lines.html">text-lines</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.command.audit.html">lookup.command.audit</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">diagnostic-line</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.command.audit/diagnostic-line.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.command.audit.html">lookup.command.audit</a>/</div>
<h1><code>diagnostic-line</code></h1>
</header>

Render one audit diagnostic as a text line.

<hr class="signature-divider">

```roo
(diagnostic-line diagnostic)
```

<hr class="signature-divider">

## Signature:

```roo
(diagnostic-line diagnostic)
```

| Arg        | Description               |
| ---------- | ------------------------- |
| diagnostic | One audit diagnostic map. |

### Returns:

A human-readable diagnostic string.

</main>
</div>
