---
layout: reference
title: proofread.rules.simple-expressions.use-argument-destructuring/matching-binding?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="default-options.html">default-options</a></li>
<li><a href="defun-body-index.html">defun-body-index</a></li>
<li><a href="destructurable-let-p.html">destructurable-let?</a></li>
<li><a class="active" href="matching-binding-p.html">matching-binding?</a></li>
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
<span aria-current="page">matching-binding?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-argument-destructuring/matching-binding-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>/</div>
<h1><code>matching-binding?</code></h1>
</header>

Test whether a let binding can move into an argument destructuring form.

<hr class="signature-divider">

```roo
(matching-binding? bindings param-name options)
```

<hr class="signature-divider">

## Signature:

```roo
(matching-binding? bindings param-name options)
```

| Arg        | Description                  |
| ---------- | ---------------------------- |
| bindings   | Let-binding vector children. |
| param-name | Function parameter name.     |
| options    | Rule options.                |

### Returns:

`true` when the binding maps a keyword lookup on `param-name` to a new symbol,
according to rule options.

</main>
</div>
