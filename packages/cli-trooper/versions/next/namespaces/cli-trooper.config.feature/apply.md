---
layout: reference
title: cli-trooper.config.feature/apply
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.feature.html">cli-trooper.config.feature</a>
<ol>
<li><a class="active" href="apply.html">apply</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.feature.html">cli-trooper.config.feature</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">apply</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.feature/apply.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.feature.html">cli-trooper.config.feature</a>/</div>
<h1><code>apply</code></h1>
</header>

Apply built-in and userspace feature processors to a canonical CLI model.

<hr class="signature-divider">

```roo
(apply config)
```

<hr class="signature-divider">

## Signature:

```roo
(apply config)
```

| Arg    | Description                                                |
| ------ | ---------------------------------------------------------- |
| config | The canonical model before feature-generated declarations. |

### Returns:

The canonical model after help, version, and every function in
`:features :processors` has transformed it in declaration order.

</main>
</div>
