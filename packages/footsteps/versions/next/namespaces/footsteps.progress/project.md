---
layout: reference
title: footsteps.progress/project
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.progress.html">footsteps.progress</a>
<ol>
<li><a href="clamp.html">clamp</a></li>
<li><a class="active" href="project.html">project</a></li>
<li><a href="projected-value.html">projected-value</a></li>
<li><a href="ratio.html">ratio</a></li>
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
<span aria-current="page">project</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.progress/project.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.progress.html">footsteps.progress</a>/</div>
<h1><code>project</code></h1>
</header>

Project child progress into a parent progress scope.

<hr class="signature-divider">

```roo
(project scope-spec progress-report)
```

<hr class="signature-divider">

## Signature:

```roo
(project scope-spec progress-report)
```

| Arg             | Description                                                                             |
| --------------- | --------------------------------------------------------------------------------------- |
| scope-spec      | Target scope with `:start`, `:end`, and optional `:progress-max`, label, and id fields. |
| progress-report | Child progress report or work value.                                                    |

### Returns:

A progress report in the parent progress coordinate space.

</main>
</div>
