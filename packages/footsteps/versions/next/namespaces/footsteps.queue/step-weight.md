---
layout: reference
title: footsteps.queue/step-weight
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.queue.html">footsteps.queue</a>
<ol>
<li><a href="allocate-instances.html">allocate-instances</a></li>
<li><a href="append-after-current.html">append-after-current</a></li>
<li><a href="append-hidden.html">append-hidden</a></li>
<li><a href="append-strategy.html">append-strategy</a></li>
<li><a href="append-within-current-scope.html">append-within-current-scope</a></li>
<li><a href="append-work.html">append-work</a></li>
<li><a href="instance.html">instance</a></li>
<li><a class="active" href="step-weight.html">step-weight</a></li>
<li><a href="total-weight.html">total-weight</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../footsteps.queue.html">footsteps.queue</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">step-weight</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.queue/step-weight.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.queue.html">footsteps.queue</a>/</div>
<h1><code>step-weight</code></h1>
</header>

Return the progress weight for a queued step entry.

<hr class="signature-divider">

```roo
(step-weight work entry step)
```

<hr class="signature-divider">

## Signature:

```roo
(step-weight work entry step)
```

| Arg   | Description                            |
| ----- | -------------------------------------- |
| work  | Parent work value.                     |
| entry | Start or append entry being allocated. |
| step  | Step configuration from `:steps`.      |

### Returns:

A numeric weight. `:weight :inherit` resolves the step estimate.

</main>
</div>
