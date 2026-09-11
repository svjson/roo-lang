---
layout: reference
title: lookup.command.index/execute!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.command.index.html">lookup.command.index</a>
<ol>
<li><a href="command.html">command</a></li>
<li><a class="active" href="execute-bang.html">execute!</a></li>
<li><a href="validation-errors.html">validation-errors</a></li>
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
<span aria-current="page">execute!</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.command.index/execute-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.command.index.html">lookup.command.index</a>/</div>
<h1><code>execute!</code></h1>
</header>

Build an index artifact and print or write the requested representation.

<hr class="signature-divider">

```roo
(execute! context options)
```

<hr class="signature-divider">

## Signature:

```roo
(execute! context options)
```

| Arg     | Description                                                  |
| ------- | ------------------------------------------------------------ |
| context | CLI invocation context; index execution does not inspect it. |
| options | Validated index command options.                             |

### Returns:

The generated index snapshot or delta.

</main>
</div>
