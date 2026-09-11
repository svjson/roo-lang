---
layout: reference
title: footsteps.queue/allocate-instances
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../footsteps.queue.html">footsteps.queue</a>
<ol>
<li><a class="active" href="allocate-instances.html">allocate-instances</a></li>
<li><a href="append-after-current.html">append-after-current</a></li>
<li><a href="append-hidden.html">append-hidden</a></li>
<li><a href="append-strategy.html">append-strategy</a></li>
<li><a href="append-within-current-scope.html">append-within-current-scope</a></li>
<li><a href="append-work.html">append-work</a></li>
<li><a href="instance.html">instance</a></li>
<li><a href="step-weight.html">step-weight</a></li>
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
<span aria-current="page">allocate-instances</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/footsteps.queue/allocate-instances.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../footsteps.queue.html">footsteps.queue</a>/</div>
<h1><code>allocate-instances</code></h1>
</header>

Allocate queued step instances across a parent progress scope.

<hr class="signature-divider">

```roo
(allocate-instances work entries parent-scope)
```

<hr class="signature-divider">

## Signature:

```roo
(allocate-instances work entries parent-scope)
```

| Arg          | Description                                    |
| ------------ | ---------------------------------------------- |
| work         | Parent work value.                             |
| entries      | Step entries to allocate.                      |
| parent-scope | Scope whose width is divided by entry weights. |

### Returns:

A vector of step instances with concrete progress scopes.

</main>
</div>
