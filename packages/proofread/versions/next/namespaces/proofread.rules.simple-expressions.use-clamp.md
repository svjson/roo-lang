---
layout: reference
title: proofread.rules.simple-expressions.use-clamp
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-clamp.html">proofread.rules.simple-expressions.use-clamp</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-clamp/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-clamp/nested-clamp-call-p.html">nested-clamp-call?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-clamp</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-clamp.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-clamp

| Symbol                                                                                        | Summary                                                          |
| --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`check`](proofread.rules.simple-expressions.use-clamp/check.html)                            | Return a `use-clamp` diagnostic for matching nodes.              |
| [`nested-clamp-call?`](proofread.rules.simple-expressions.use-clamp/nested-clamp-call-p.html) | Test whether a `min` or `max` call wraps the opposite operation. |

</main>
</div>
