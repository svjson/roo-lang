---
layout: reference
title: footsteps.work/advance
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.work.html">footsteps.work</a>
<ol>
<li><a class="active" href="advance.html">advance</a></li>
<li><a href="apply-delegated-work.html">apply-delegated-work</a></li>
<li><a href="apply-step-result.html">apply-step-result</a></li>
<li><a href="complete-current.html">complete-current</a></li>
<li><a href="complete-p.html">complete?</a></li>
<li><a href="completed-if-drained.html">completed-if-drained</a></li>
<li><a href="continue-delegated-work.html">continue-delegated-work</a></li>
<li><a href="default-work-slice-limit.html">default-work-slice-limit</a></li>
<li><a href="finish-current.html">finish-current</a></li>
<li><a href="make.html">make</a></li>
<li><a href="run.html">run</a></li>
<li><a href="slice.html">slice</a></li>
<li><a href="start-current.html">start-current</a></li>
<li><a href="with-delegated-work.html">with-delegated-work</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.work.html">footsteps.work</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">advance</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.work/advance.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.work.html">footsteps.work</a>/</div>
<h1><code>advance</code></h1>
</header>

Advance a work value by one observable step.

<hr class="signature-divider">

```roo
(advance work)
```

<hr class="signature-divider">

## Signature:

```roo
(advance work)
```

| Arg  | Description                                                          |
| ---- | -------------------------------------------------------------------- |
| work | Work value returned by `make` or a previous `advance`/`slice`/`run`. |

### Returns:

A result map containing `:work`, `:complete?`, `:result`, `:progress`, and
optional `:metadata` from the step.

</main>
</div>
