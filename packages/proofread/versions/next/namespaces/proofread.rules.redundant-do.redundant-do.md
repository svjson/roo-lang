---
layout: reference
title: proofread.rules.redundant-do.redundant-do
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>
<ol>
<li><a href="proofread.rules.redundant-do.redundant-do/body-start-index.html">body-start-index</a></li>
<li><a href="proofread.rules.redundant-do.redundant-do/check.html">check</a></li>
<li><a href="proofread.rules.redundant-do.redundant-do/redundant-do-p.html">redundant-do?</a></li>
<li><a href="proofread.rules.redundant-do.redundant-do/sole-body-form-p.html">sole-body-form?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.redundant-do.redundant-do</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.redundant-do.redundant-do.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.redundant-do.redundant-do

| Symbol                                                                                | Summary                                                           |
| ------------------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| [`body-start-index`](proofread.rules.redundant-do.redundant-do/body-start-index.html) | Return the child index where a form's implicit body starts.       |
| [`check`](proofread.rules.redundant-do.redundant-do/check.html)                       | Return a `redundant-do` diagnostic for matching nodes.            |
| [`redundant-do?`](proofread.rules.redundant-do.redundant-do/redundant-do-p.html)      | Test whether an AST node is a redundant `do` form.                |
| [`sole-body-form?`](proofread.rules.redundant-do.redundant-do/sole-body-form-p.html)  | Test whether `node` is the only form in `parent`'s implicit body. |

</main>
</div>
