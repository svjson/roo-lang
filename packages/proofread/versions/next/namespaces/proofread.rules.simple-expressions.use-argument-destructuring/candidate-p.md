---
layout: reference
title: proofread.rules.simple-expressions.use-argument-destructuring/candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>
<ol>
<li><a class="active" href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="default-options.html">default-options</a></li>
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
<span aria-current="page">candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-argument-destructuring/candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>/</div>
<h1><code>candidate?</code></h1>
</header>

Test whether an immediate function-body `let` should be argument destructuring.

<hr class="signature-divider">

```roo
(candidate? parent node options)
```

<hr class="signature-divider">

## Signature:

```roo
(candidate? parent node options)
```

| Arg     | Description |
| ------- | ----------- |
| parent  |             |
| node    |             |
| options |             |

</main>
</div>
