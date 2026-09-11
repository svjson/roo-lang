---
layout: reference
title: proofread.rules.simple-expressions.use-any
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-any.html">proofread.rules.simple-expressions.use-any</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-any/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-any/find-first-form-p.html">find-first-form?</a></li>
<li><a href="proofread.rules.simple-expressions.use-any/find-index-form-p.html">find-index-form?</a></li>
<li><a href="proofread.rules.simple-expressions.use-any/presence-check-p.html">presence-check?</a></li>
<li><a href="proofread.rules.simple-expressions.use-any/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-any</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-any.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-any

| Symbol                                                                                  | Summary                                                            |
| --------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`check`](proofread.rules.simple-expressions.use-any/check.html)                        | Report search calls used only as presence checks.                  |
| [`find-first-form?`](proofread.rules.simple-expressions.use-any/find-first-form-p.html) | Test whether an AST node is a `(find-first value predicate)` form. |
| [`find-index-form?`](proofread.rules.simple-expressions.use-any/find-index-form-p.html) | Test whether an AST node is a `(find-index value predicate)` form. |
| [`presence-check?`](proofread.rules.simple-expressions.use-any/presence-check-p.html)   | Test whether an AST node checks value nil-ness.                    |
| [`replacement`](proofread.rules.simple-expressions.use-any/replacement.html)            | Return the replacement for a boolean `find-*` check.               |

</main>
</div>
