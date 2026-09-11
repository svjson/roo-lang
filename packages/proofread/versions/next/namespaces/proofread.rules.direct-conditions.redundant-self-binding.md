---
layout: reference
title: proofread.rules.direct-conditions.redundant-self-binding
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.direct-conditions.redundant-self-binding.html">proofread.rules.direct-conditions.redundant-self-binding</a>
<ol>
<li><a href="proofread.rules.direct-conditions.redundant-self-binding/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.direct-conditions.redundant-self-binding/check.html">check</a></li>
<li><a href="proofread.rules.direct-conditions.redundant-self-binding/self-binding-p.html">self-binding?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.direct-conditions.redundant-self-binding</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.redundant-self-binding.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.direct-conditions.redundant-self-binding

| Symbol                                                                                          | Summary                                                               |
| ----------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`candidate?`](proofread.rules.direct-conditions.redundant-self-binding/candidate-p.html)       | Test whether an `if-let` or `when-let` uses a redundant self-binding. |
| [`check`](proofread.rules.direct-conditions.redundant-self-binding/check.html)                  | Return a `redundant-self-binding` diagnostic for matching nodes.      |
| [`self-binding?`](proofread.rules.direct-conditions.redundant-self-binding/self-binding-p.html) | Test whether a binding vector binds a symbol to itself.               |

</main>
</div>
