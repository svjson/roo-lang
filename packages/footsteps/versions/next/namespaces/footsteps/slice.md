---
layout: reference
title: footsteps/slice
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.html">footsteps</a>
<ol>
<li><a href="advance.html">advance</a></li>
<li><a href="append-strategy.html">append-strategy</a></li>
<li><a href="complete-p.html">complete?</a></li>
<li><a href="estimate.html">estimate</a></li>
<li><a href="make.html">make</a></li>
<li><a href="progress.html">progress</a></li>
<li><a href="progress-report.html">progress-report</a></li>
<li><a href="project-progress.html">project-progress</a></li>
<li><a href="run.html">run</a></li>
<li><a class="active" href="slice.html">slice</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.html">footsteps</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">slice</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps/slice.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.html">footsteps</a>/</div>
<h1><code>slice</code></h1>
</header>

Advance work for one cooperative time slice.

<hr class="signature-divider">

```roo
(slice work & opts &rest)
```

<hr class="signature-divider">

## Signature:

```roo
(slice work & opts &rest)
```

| Arg   | Description                                                                                                              |
| ----- | ------------------------------------------------------------------------------------------------------------------------ |
| work  | Work value returned by `make` or a previous `advance`/`slice`/`run`.                                                     |
| opts  | Optional map. Supports `:clock`, `:target-duration-ms`, legacy `:min-duration-ms`, `:max-duration-ms`, and `:max-steps`. |
| &rest |                                                                                                                          |

### Returns:

A result map like `advance`, plus `:steps-run`, `:elapsed-ms`, and `:stop-reason`.

</main>
</div>
