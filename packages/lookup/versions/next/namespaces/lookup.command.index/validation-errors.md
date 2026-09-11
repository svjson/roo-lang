---
layout: reference
title: lookup.command.index/validation-errors
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.command.index.html">lookup.command.index</a>
<ol>
<li><a href="command.html">command</a></li>
<li><a href="execute-bang.html">execute!</a></li>
<li><a class="active" href="validation-errors.html">validation-errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.command.index.html">lookup.command.index</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">validation-errors</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.command.index/validation-errors.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.command.index.html">lookup.command.index</a>/</div>
<h1><code>validation-errors</code></h1>
</header>

Validate semantic constraints between lookup index options.

<hr class="signature-divider">

```roo
(validation-errors options)
```

<hr class="signature-divider">

## Signature:

```roo
(validation-errors options)
```

| Arg     | Description                         |
| ------- | ----------------------------------- |
| options | Fully parsed index command options. |

### Returns:

A vector containing the first semantic diagnostic, or an empty vector when
the options describe a supported indexing operation.

</main>
</div>
