---
layout: reference
title: cli-trooper.invoke/resolve-execution
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.invoke.html">cli-trooper.invoke</a>
<ol>
<li><a href="command-flag-execution.html">command-flag-execution</a></li>
<li><a class="active" href="resolve-execution.html">resolve-execution</a></li>
<li><a href="run.html">run</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.invoke.html">cli-trooper.invoke</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">resolve-execution</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.invoke/resolve-execution.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.invoke.html">cli-trooper.invoke</a>/</div>
<h1><code>resolve-execution</code></h1>
</header>

Resolve the execution function for a selected command identity.

<hr class="signature-divider">

```roo
(resolve-execution config command)
```

<hr class="signature-divider">

## Signature:

```roo
(resolve-execution config command)
```

| Arg     | Description                                                                                                                                               |
| ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| config  | A canonical cli-trooper configuration model.                                                                                                              |
| command | The empty root path, an ordinary command identity, a root<br>command-shaped flag identity, or a command path ending in a command-shaped<br>flag identity. |

### Returns:

The selected `:execute!` function, or `nil` when the declaration has none.

</main>
</div>
