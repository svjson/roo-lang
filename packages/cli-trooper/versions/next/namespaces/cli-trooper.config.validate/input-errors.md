---
layout: reference
title: cli-trooper.config.validate/input-errors
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>
<ol>
<li><a href="flag-errors.html">flag-errors</a></li>
<li><a class="active" href="input-errors.html">input-errors</a></li>
<li><a href="input-flag-errors.html">input-flag-errors</a></li>
<li><a href="missing-mixin-errors.html">missing-mixin-errors</a></li>
<li><a href="model-errors.html">model-errors</a></li>
<li><a href="validate-flag-entry.html">validate-flag-entry</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">input-errors</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.validate/input-errors.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>/</div>
<h1><code>input-errors</code></h1>
</header>

Validate concerns that must be detected before shorthand is discarded.

<hr class="signature-divider">

```roo
(input-errors config)
```

<hr class="signature-divider">

## Signature:

```roo
(input-errors config)
```

| Arg    | Description                             |
| ------ | --------------------------------------- |
| config | The supplied application configuration. |

### Returns:

A sequence of unknown-mixin and supplied flag-collision diagnostics.

</main>
</div>
