---
layout: reference
title: footsteps.progress/ratio
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.progress.html">footsteps.progress</a>
<ol>
<li><a href="clamp.html">clamp</a></li>
<li><a href="project.html">project</a></li>
<li><a href="projected-value.html">projected-value</a></li>
<li><a class="active" href="ratio.html">ratio</a></li>
<li><a href="raw-value.html">raw-value</a></li>
<li><a href="report.html">report</a></li>
<li><a href="value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.progress.html">footsteps.progress</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">ratio</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.progress/ratio.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.progress.html">footsteps.progress</a>/</div>
<h1><code>ratio</code></h1>
</header>

Return a clamped progress ratio for a report.

<hr class="signature-divider">

```roo
(ratio progress-report)
```

<hr class="signature-divider">

## Signature:

```roo
(ratio progress-report)
```

| Arg             | Description                    |
| --------------- | ------------------------------ |
| progress-report | Progress report or work value. |

### Returns:

A number from `0` to `1`.

</main>
</div>
