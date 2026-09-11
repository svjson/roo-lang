---
layout: reference
title: cli-trooper.config.command/normalize
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.command.html">cli-trooper.config.command</a>
<ol>
<li><a href="mixed-flags.html">mixed-flags</a></li>
<li><a class="active" href="normalize.html">normalize</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.command.html">cli-trooper.config.command</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">normalize</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.command/normalize.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.command.html">cli-trooper.config.command</a>/</div>
<h1><code>normalize</code></h1>
</header>

Normalize one command while retaining unrecognized prototype metadata.

<hr class="signature-divider">

```roo
(normalize config name spec)
```

<hr class="signature-divider">

## Signature:

```roo
(normalize config name spec)
```

| Arg    | Description                             |
| ------ | --------------------------------------- |
| config | The supplied application configuration. |
| name   | The command's literal name.             |
| spec   | The supplied command specification.     |

### Returns:

A canonical command map with expanded flags and positionals and no `:mixin`.

</main>
</div>
