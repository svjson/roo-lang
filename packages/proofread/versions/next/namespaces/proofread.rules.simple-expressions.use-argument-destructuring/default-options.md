---
layout: reference
title: proofread.rules.simple-expressions.use-argument-destructuring/default-options
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a class="active" href="default-options.html">default-options</a></li>
<li><a href="defun-body-index.html">defun-body-index</a></li>
<li><a href="destructurable-let-p.html">destructurable-let?</a></li>
<li><a href="matching-binding-p.html">matching-binding?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">default-options</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-argument-destructuring/default-options.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>/</div>
<h1><code>default-options</code></h1>
</header>

Return the default argument-destructuring configuration.

<hr class="signature-divider">

```roo
(default-options)
```

<hr class="signature-divider">

### Returns:

A map controlling whether renamed keyword lookups are allowed.

<hr class="signature-divider">

## Signature:

```roo
(default-options)
```

### Returns:

A map controlling whether renamed keyword lookups are allowed.

</main>
</div>
