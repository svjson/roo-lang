---
layout: reference
title: cli-trooper.config/normalize-result
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.html">cli-trooper.config</a>
<ol>
<li><a href="normalize.html">normalize</a></li>
<li><a class="active" href="normalize-result.html">normalize-result</a></li>
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
<span aria-current="page">normalize-result</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config/normalize-result.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.html">cli-trooper.config</a>/</div>
<h1><code>normalize-result</code></h1>
</header>

Normalize configuration and collect structured configuration diagnostics.

<hr class="signature-divider">

```roo
(normalize-result config)
```

<hr class="signature-divider">

## Signature:

```roo
(normalize-result config)
```

| Arg    | Description                                 |
| ------ | ------------------------------------------- |
| config | The supplied cli-trooper configuration map. |

### Returns:

A map containing the normalized model under `:config` and a vector of
diagnostics under `:errors`.

</main>
</div>
