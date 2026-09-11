---
layout: reference
title: footsteps.scope/size
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.scope.html">footsteps.scope</a>
<ol>
<li><a href="default-progress-max.html">default-progress-max</a></li>
<li><a href="make.html">make</a></li>
<li><a class="active" href="size.html">size</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.scope.html">footsteps.scope</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">size</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.scope/size.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.scope.html">footsteps.scope</a>/</div>
<h1><code>size</code></h1>
</header>

Return the non-negative size of a progress scope.

<hr class="signature-divider">

```roo
(size scope)
```

<hr class="signature-divider">

## Signature:

```roo
(size scope)
```

| Arg   | Description                         |
| ----- | ----------------------------------- |
| scope | Scope map with `:start` and `:end`. |

### Returns:

`:end - :start`, clamped to zero.

</main>
</div>
