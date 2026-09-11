---
layout: reference
title: proofread.rules.simple-expressions.use-string-starts-with
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-string-starts-with.html">proofread.rules.simple-expressions.use-string-starts-with</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/case-candidate-p.html">case-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/equality-candidate-p.html">equality-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/leading-substr-call-p.html">leading-substr-call?</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/leading-substr-source.html">leading-substr-source</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/replacement.html">replacement</a></li>
<li><a href="proofread.rules.simple-expressions.use-string-starts-with/trailing-substr-call-p.html">trailing-substr-call?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-string-starts-with</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-string-starts-with.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-string-starts-with

| Symbol                                                                                                           | Summary                                                                |
| ---------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| [`candidate?`](proofread.rules.simple-expressions.use-string-starts-with/candidate-p.html)                       | Return candidate metadata for `string/starts-with?` suggestions.       |
| [`case-candidate?`](proofread.rules.simple-expressions.use-string-starts-with/case-candidate-p.html)             | Return start-with candidate info from a `case` form.                   |
| [`check`](proofread.rules.simple-expressions.use-string-starts-with/check.html)                                  | Report `string/starts-with?` opportunities in equality and case forms. |
| [`equality-candidate?`](proofread.rules.simple-expressions.use-string-starts-with/equality-candidate-p.html)     | Return start-with candidate info from an equality form.                |
| [`leading-substr-call?`](proofread.rules.simple-expressions.use-string-starts-with/leading-substr-call-p.html)   | Test whether a form is `(substr value 0 1)`.                           |
| [`leading-substr-source`](proofread.rules.simple-expressions.use-string-starts-with/leading-substr-source.html)  | Return the source argument for `(substr value 0 1)`.                   |
| [`replacement`](proofread.rules.simple-expressions.use-string-starts-with/replacement.html)                      | Return a source replacement string for `string/starts-with?`.          |
| [`trailing-substr-call?`](proofread.rules.simple-expressions.use-string-starts-with/trailing-substr-call-p.html) | Test whether a form removes the first character from the same source.  |

</main>
</div>
