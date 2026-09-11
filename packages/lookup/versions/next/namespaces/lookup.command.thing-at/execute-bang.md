---
layout: reference
title: lookup.command.thing-at/execute!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.command.thing-at.html">lookup.command.thing-at</a>
<ol>
<li><a href="command.html">command</a></li>
<li><a class="active" href="execute-bang.html">execute!</a></li>
<li><a href="resolve-namespace.html">resolve-namespace</a></li>
<li><a href="with-namespace-fallback.html">with-namespace-fallback</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.command.thing-at.html">lookup.command.thing-at</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">execute!</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.command.thing-at/execute-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.command.thing-at.html">lookup.command.thing-at</a>/</div>
<h1><code>execute!</code></h1>
</header>

Resolve and print the Roo entity at a source position.

<hr class="signature-divider">

```roo
(execute! context options)
```

<hr class="signature-divider">

## Signature:

```roo
(execute! context options)
```

| Arg     | Description                                                     |
| ------- | --------------------------------------------------------------- |
| context | CLI invocation context; thing-at execution does not inspect it. |
| options | Parsed thing-at command options.                                |

### Returns:

The analysis result, with namespace fallback resolution applied.

</main>
</div>
