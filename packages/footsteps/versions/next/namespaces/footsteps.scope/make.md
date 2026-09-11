---
layout: reference
title: footsteps.scope/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.scope.html">footsteps.scope</a>
<ol>
<li><a href="default-progress-max.html">default-progress-max</a></li>
<li><a class="active" href="make.html">make</a></li>
<li><a href="size.html">size</a></li>
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
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.scope/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.scope.html">footsteps.scope</a>/</div>
<h1><code>make</code></h1>
</header>

Create a progress scope.

<hr class="signature-divider">

```roo
(make id start end)
```

<hr class="signature-divider">

## Signature:

```roo
(make id start end)
```

| Arg   | Description                                     |
| ----- | ----------------------------------------------- |
| id    | Scope identifier.                               |
| start | Inclusive start value in parent progress space. |
| end   | End value in parent progress space.             |

### Returns:

A scope map with `:id`, `:start`, and `:end`.

</main>
</div>
