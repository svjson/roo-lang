---
layout: reference
title: cli-trooper.config.feature.help/declaration
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.feature.help.html">cli-trooper.config.feature.help</a>
<ol>
<li><a href="apply.html">apply</a></li>
<li><a class="active" href="declaration.html">declaration</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.feature.help.html">cli-trooper.config.feature.help</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">declaration</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.feature.help/declaration.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.feature.help.html">cli-trooper.config.feature.help</a>/</div>
<h1><code>declaration</code></h1>
</header>

Build one canonical help command flag declaration.

<hr class="signature-divider">

```roo
(declaration config inherited?)
```

<hr class="signature-divider">

## Signature:

```roo
(declaration config inherited?)
```

| Arg        | Description                                             |
| ---------- | ------------------------------------------------------- |
| config     | Effective help configuration for the attachment level.  |
| inherited? | Whether descendant command states may inherit the flag. |

### Returns:

A map containing normalized help flag names under `:names` and the canonical
command-shaped flag specification under `:spec`.

</main>
</div>
