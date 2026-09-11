---
layout: reference
title: cli-trooper.parse.argument/consume
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.argument.html">cli-trooper.parse.argument</a>
<ol>
<li><a class="active" href="consume.html">consume</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.argument.html">cli-trooper.parse.argument</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">consume</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.argument/consume.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.argument.html">cli-trooper.parse.argument</a>/</div>
<h1><code>consume</code></h1>
</header>

Apply one argument to the current parser state.

<hr class="signature-divider">

```roo
(consume selection state arg)
```

<hr class="signature-divider">

## Signature:

```roo
(consume selection state arg)
```

| Arg       | Description                                        |
| --------- | -------------------------------------------------- |
| selection | The selected command, literal, and argument index. |
| state     | The current argument parser state.                 |
| arg       | The raw argument to interpret.                     |

### Returns:

Parser state with the argument interpreted and `:arg-index` advanced.

</main>
</div>
