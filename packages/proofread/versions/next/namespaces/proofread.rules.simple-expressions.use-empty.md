---
layout: reference
title: proofread.rules.simple-expressions.use-empty
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-empty/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-empty/count-form-argument.html">count-form-argument</a></li>
<li><a href="proofread.rules.simple-expressions.use-empty/empty-count-comparison-p.html">empty-count-comparison?</a></li>
<li><a href="proofread.rules.simple-expressions.use-empty/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-empty</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-empty.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-empty

| Symbol                                                                                                  | Summary                                                               |
| ------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-empty/check.html)                                      | Report count comparisons that should use `empty?`.                    |
| [`count-form-argument`](proofread.rules.simple-expressions.use-empty/count-form-argument.html)          | Return the argument inside a `count` call in a count/zero comparison. |
| [`empty-count-comparison?`](proofread.rules.simple-expressions.use-empty/empty-count-comparison-p.html) | Test whether an AST node compares a count to zero.                    |
| [`replacement`](proofread.rules.simple-expressions.use-empty/replacement.html)                          | Return the replacement for a count/zero comparison.                   |

</main>
</div>
