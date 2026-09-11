---
layout: reference
title: cli-trooper.config/normalize
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.html">cli-trooper.config</a>
<ol>
<li><a class="active" href="normalize.html">normalize</a></li>
<li><a href="normalize-result.html">normalize-result</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.html">cli-trooper.config</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">normalize</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config/normalize.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.html">cli-trooper.config</a>/</div>
<h1><code>normalize</code></h1>
</header>

Normalize ergonomic CLI configuration into the canonical public model.

<hr class="signature-divider">

```roo
(normalize config)
```

<hr class="signature-divider">

## Signature:

```roo
(normalize config)
```

| Arg    | Description                                 |
| ------ | ------------------------------------------- |
| config | The supplied cli-trooper configuration map. |

### Returns:

The normalized public configuration model. Configuration diagnostics are not
included; use `normalize-result` when they are required.

</main>
</div>
