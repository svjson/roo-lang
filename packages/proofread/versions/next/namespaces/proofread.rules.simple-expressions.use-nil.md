---
layout: reference
title: proofread.rules.simple-expressions.use-nil
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-nil.html">proofread.rules.simple-expressions.use-nil</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-nil/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-nil/nil-comparison-value.html">nil-comparison-value</a></li>
<li><a href="proofread.rules.simple-expressions.use-nil/nil-comparison-p.html">nil-comparison?</a></li>
<li><a href="proofread.rules.simple-expressions.use-nil/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-nil</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-nil.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-nil

| Symbol                                                                                         | Summary                                             |
| ---------------------------------------------------------------------------------------------- | --------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-nil/check.html)                               | Report equality checks against `nil`.               |
| [`nil-comparison-value`](proofread.rules.simple-expressions.use-nil/nil-comparison-value.html) | Return the non-nil side of a nil comparison.        |
| [`nil-comparison?`](proofread.rules.simple-expressions.use-nil/nil-comparison-p.html)          | Test whether an AST node compares a value to `nil`. |
| [`replacement`](proofread.rules.simple-expressions.use-nil/replacement.html)                   | Return the replacement for `(= nil value)`.         |

</main>
</div>
