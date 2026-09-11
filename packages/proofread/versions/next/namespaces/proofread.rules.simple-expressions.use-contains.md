---
layout: reference
title: proofread.rules.simple-expressions.use-contains
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-contains.html">proofread.rules.simple-expressions.use-contains</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-contains/any-equality-membership-reversed-p.html">any-equality-membership-reversed?</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/any-equality-membership-p.html">any-equality-membership?</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/equality-against-param-p.html">equality-against-param?</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/equality-membership-predicate-p.html">equality-membership-predicate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/index-of-form-p.html">index-of-form?</a></li>
<li><a href="proofread.rules.simple-expressions.use-contains/presence-check-p.html">presence-check?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-contains</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-contains.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-contains

| Symbol                                                                                                                         | Summary                                                                            |
| ------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------- |
| [`any-equality-membership-reversed?`](proofread.rules.simple-expressions.use-contains/any-equality-membership-reversed-p.html) | Test whether `any?` performs a simple equality membership scan with reversed args. |
| [`any-equality-membership?`](proofread.rules.simple-expressions.use-contains/any-equality-membership-p.html)                   | Test whether `any?` performs a simple equality membership scan.                    |
| [`check`](proofread.rules.simple-expressions.use-contains/check.html)                                                          | Report membership checks that should use `contains?`.                              |
| [`equality-against-param?`](proofread.rules.simple-expressions.use-contains/equality-against-param-p.html)                     | Test whether an equality compares directly against a parameter.                    |
| [`equality-membership-predicate?`](proofread.rules.simple-expressions.use-contains/equality-membership-predicate-p.html)       | Test whether an inline predicate is a simple membership equality.                  |
| [`index-of-form?`](proofread.rules.simple-expressions.use-contains/index-of-form-p.html)                                       | Test whether an AST node is an `(index-of value needle)` form.                     |
| [`presence-check?`](proofread.rules.simple-expressions.use-contains/presence-check-p.html)                                     | Test whether an AST node checks value nil-ness.                                    |

</main>
</div>
