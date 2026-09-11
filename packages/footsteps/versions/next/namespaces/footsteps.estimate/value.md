---
layout: reference
title: footsteps.estimate/value
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.estimate.html">footsteps.estimate</a>
<ol>
<li><a class="active" href="value.html">value</a></li>
<li><a href="value-star.html">value*</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.estimate.html">footsteps.estimate</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">value</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.estimate/value.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.estimate.html">footsteps.estimate</a>/</div>
<h1><code>value</code></h1>
</header>

Resolve a literal, function, or step-map estimate.

<hr class="signature-divider">

```roo
(value candidate ctx)
```

<hr class="signature-divider">

## Signature:

```roo
(value candidate ctx)
```

| Arg       | Description                                                                                                                                  |
| --------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| candidate | Literal estimate, function estimate, or step map containing `:estimate`, `:estimated-duration-ms`, `:progress-estimate`, or `:progress-max`. |
| ctx       | Context map passed to estimate functions.                                                                                                    |

### Returns:

The resolved estimate value. Step maps without estimate metadata return `1`.

</main>
</div>
