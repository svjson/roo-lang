---
layout: reference
title: proofread.rules.simple-expressions.use-some
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-some.html">proofread.rules.simple-expressions.use-some</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-some/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-some/direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-some/nil-check-p.html">nil-check?</a></li>
<li><a href="proofread.rules.simple-expressions.use-some/not-nil-check-p.html">not-nil-check?</a></li>
<li><a href="proofread.rules.simple-expressions.use-some/replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-some</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-some.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-some

| Symbol                                                                                                   | Summary                                                                         |
| -------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-some/check.html)                                        | Report negated `nil?` checks.                                                   |
| [`direct-unless-candidate?`](proofread.rules.simple-expressions.use-some/direct-unless-candidate-p.html) | Test whether this negated expression should be handled by `use-unless` instead. |
| [`nil-check?`](proofread.rules.simple-expressions.use-some/nil-check-p.html)                             | Test whether an AST node is a `(nil? value)` form.                              |
| [`not-nil-check?`](proofread.rules.simple-expressions.use-some/not-nil-check-p.html)                     | Test whether an AST node negates a `nil?` check.                                |
| [`replacement`](proofread.rules.simple-expressions.use-some/replacement.html)                            | Return the replacement for `(not (nil? value))`.                                |

</main>
</div>
