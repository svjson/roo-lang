---
layout: reference
title: proofread.rules.simple-expressions.use-keyword-lookup
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-keyword-lookup.html">proofread.rules.simple-expressions.use-keyword-lookup</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/defaulted-get-fallback-p.html">defaulted-get-fallback?</a></li>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/get-default-child-p.html">get-default-child?</a></li>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/get-form-p.html">get-form?</a></li>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/keyword-get-p.html">keyword-get?</a></li>
<li><a href="proofread.rules.simple-expressions.use-keyword-lookup/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-keyword-lookup</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-keyword-lookup.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-keyword-lookup

| Symbol                                                                                                           | Summary                                                          |
| ---------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-keyword-lookup/check.html)                                      | Report `(get value :key)` forms.                                 |
| [`defaulted-get-fallback?`](proofread.rules.simple-expressions.use-keyword-lookup/defaulted-get-fallback-p.html) | Test whether an AST node defaults a `get` with `or`.             |
| [`get-default-child?`](proofread.rules.simple-expressions.use-keyword-lookup/get-default-child-p.html)           | Test whether `node` is the `get` child in a defaulted `or` form. |
| [`get-form?`](proofread.rules.simple-expressions.use-keyword-lookup/get-form-p.html)                             | Test whether an AST node is a two-argument `get` form.           |
| [`keyword-get?`](proofread.rules.simple-expressions.use-keyword-lookup/keyword-get-p.html)                       | Test whether an AST node gets a literal keyword key.             |
| [`replacement`](proofread.rules.simple-expressions.use-keyword-lookup/replacement.html)                          | Return the replacement for `(get value :key)`.                   |

</main>
</div>
