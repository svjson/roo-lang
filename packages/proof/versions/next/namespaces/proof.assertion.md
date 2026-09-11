---
layout: reference
title: proof.assertion
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.assertion.html">proof.assertion</a>
<ol>
<li><a href="proof.assertion/diff-lines.html">diff-lines</a></li>
<li><a href="proof.assertion/equality-fail-message.html">equality-fail-message</a></li>
<li><a href="proof.assertion/format-failure-message.html">format-failure-message</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.assertion</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.assertion.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.assertion

| Symbol                                                                  | Summary                                                            |
| ----------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`diff-lines`](proof.assertion/diff-lines.html)                         | Return rendered line-by-line diff lines for pretty-printed values. |
| [`equality-fail-message`](proof.assertion/equality-fail-message.html)   | Format an equality assertion failure message.                      |
| [`format-failure-message`](proof.assertion/format-failure-message.html) | Format a failed assertion message.                                 |

</main>
</div>
