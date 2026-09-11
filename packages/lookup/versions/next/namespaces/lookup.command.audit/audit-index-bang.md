---
layout: reference
title: lookup.command.audit/audit-index!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.command.audit.html">lookup.command.audit</a>
<ol>
<li><a class="active" href="audit-index-bang.html">audit-index!</a></li>
<li><a href="command.html">command</a></li>
<li><a href="diagnostic-extra.html">diagnostic-extra</a></li>
<li><a href="diagnostic-line.html">diagnostic-line</a></li>
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
<span aria-current="page">audit-index!</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.command.audit/audit-index-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.command.audit.html">lookup.command.audit</a>/</div>
<h1><code>audit-index!</code></h1>
</header>

Load or build the symbol index selected by audit options.

<hr class="signature-divider">

```roo
(audit-index! {:keys [input] :as options})
```

<hr class="signature-divider">

## Signature:

```roo
(audit-index! {:keys [input] :as options})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [input] :as options} |             |

### Returns:

A Roo symbol index artifact. Raises an error when the input does not exist.

- `options`: Audit options containing `:input`, source roots, and exclusions.

### Returns:

A Roo symbol index artifact. Raises an error when the input does not exist.

</main>
</div>
