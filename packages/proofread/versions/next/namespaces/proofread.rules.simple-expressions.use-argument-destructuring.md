---
layout: reference
title: proofread.rules.simple-expressions.use-argument-destructuring
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-argument-destructuring.html">proofread.rules.simple-expressions.use-argument-destructuring</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/default-options.html">default-options</a></li>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/defun-body-index.html">defun-body-index</a></li>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/destructurable-let-p.html">destructurable-let?</a></li>
<li><a href="proofread.rules.simple-expressions.use-argument-destructuring/matching-binding-p.html">matching-binding?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-argument-destructuring</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-argument-destructuring.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-argument-destructuring

| Symbol                                                                                                           | Summary                                                                         |
| ---------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| [`candidate?`](proofread.rules.simple-expressions.use-argument-destructuring/candidate-p.html)                   | Test whether an immediate function-body `let` should be argument destructuring. |
| [`check`](proofread.rules.simple-expressions.use-argument-destructuring/check.html)                              | Return a `use-argument-destructuring` diagnostic using configured rule options. |
| [`default-options`](proofread.rules.simple-expressions.use-argument-destructuring/default-options.html)          | Return the default argument-destructuring configuration.                        |
| [`defun-body-index`](proofread.rules.simple-expressions.use-argument-destructuring/defun-body-index.html)        | Return the first body child index for a `defun` form.                           |
| [`destructurable-let?`](proofread.rules.simple-expressions.use-argument-destructuring/destructurable-let-p.html) | Test whether a `let` binds a keyword lookup from any parameter.                 |
| [`matching-binding?`](proofread.rules.simple-expressions.use-argument-destructuring/matching-binding-p.html)     | Test whether a let binding can move into an argument destructuring form.        |

</main>
</div>
