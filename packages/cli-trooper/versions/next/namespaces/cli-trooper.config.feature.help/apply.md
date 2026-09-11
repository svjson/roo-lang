---
layout: reference
title: cli-trooper.config.feature.help/apply
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.feature.help.html">cli-trooper.config.feature.help</a>
<ol>
<li><a class="active" href="apply.html">apply</a></li>
<li><a href="declaration.html">declaration</a></li>
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
<span aria-current="page">apply</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.feature.help/apply.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.feature.help.html">cli-trooper.config.feature.help</a>/</div>
<h1><code>apply</code></h1>
</header>

Attach configured help command flags to a canonical CLI model.

<hr class="signature-divider">

```roo
(apply config)
```

<hr class="signature-divider">

## Signature:

```roo
(apply config)
```

| Arg    | Description                                                                                                                                               |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| config | A canonical CLI model whose `:features :help` entry optionally<br>configures help attachment and whose commands may carry local `:help`<br>configuration. |

### Returns:

The canonical model with help command flags attached at the root and,
by default, every command level. Existing declarations with the exact same
canonical name vector take precedence.

</main>
</div>
