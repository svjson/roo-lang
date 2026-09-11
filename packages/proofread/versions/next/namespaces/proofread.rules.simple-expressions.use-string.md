---
layout: reference
title: proofread.rules.simple-expressions.use-string
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-string/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-string/some-symbol-name.html">some-symbol-name</a></li>
<li><a href="proofread.rules.simple-expressions.use-string/string-guard-some-check-p.html">string-guard-some-check?</a></li>
<li><a href="proofread.rules.simple-expressions.use-string/string-operation-on-p.html">string-operation-on?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-string</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-string.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-string

| Symbol                                                                                                     | Summary                                                        |
| ---------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-string/check.html)                                        | Report `some?` guards that should use `string?`.               |
| [`some-symbol-name`](proofread.rules.simple-expressions.use-string/some-symbol-name.html)                  | Return the checked symbol name from a `some?` form.            |
| [`string-guard-some-check?`](proofread.rules.simple-expressions.use-string/string-guard-some-check-p.html) | Test whether `some?` guards a string operation.                |
| [`string-operation-on?`](proofread.rules.simple-expressions.use-string/string-operation-on-p.html)         | Test whether an AST node calls a string operation on a symbol. |

</main>
</div>
