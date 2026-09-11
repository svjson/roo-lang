---
layout: reference
title: cli-trooper.parse.validation/errors
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.validation.html">cli-trooper.parse.validation</a>
<ol>
<li><a class="active" href="errors.html">errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.validation.html">cli-trooper.parse.validation</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">errors</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.validation/errors.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.validation.html">cli-trooper.parse.validation</a>/</div>
<h1><code>errors</code></h1>
</header>

Evaluate a command's semantic validator.

<hr class="signature-divider">

```roo
(errors command spec options)
```

<hr class="signature-divider">

## Signature:

```roo
(errors command spec options)
```

| Arg     | Description                                   |
| ------- | --------------------------------------------- |
| command | The selected command identity.                |
| spec    | The selected canonical command specification. |
| options | The fully resolved options map.               |

### Returns:

A sequence of structured diagnostics carrying the selected command identity,
or an empty sequence when the command has no validator.

</main>
</div>
