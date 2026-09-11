---
layout: reference
title: proofread.rules.diagnostics
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>
<ol>
<li><a href="proofread.rules.diagnostics/edit.html">edit</a></li>
<li><a href="proofread.rules.diagnostics/make.html">make</a></li>
<li><a href="proofread.rules.diagnostics/make-file.html">make-file</a></li>
<li><a href="proofread.rules.diagnostics/make-suggesting.html">make-suggesting</a></li>
<li><a href="proofread.rules.diagnostics/rewrite.html">rewrite</a></li>
<li><a href="proofread.rules.diagnostics/with-suggestions.html">with-suggestions</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.diagnostics</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.diagnostics.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.diagnostics

| Symbol                                                                  | Summary                                                        |
| ----------------------------------------------------------------------- | -------------------------------------------------------------- |
| [`edit`](proofread.rules.diagnostics/edit.html)                         | Build a source replacement edit.                               |
| [`make`](proofread.rules.diagnostics/make.html)                         | Build a proofread rule diagnostic for an AST node.             |
| [`make-file`](proofread.rules.diagnostics/make-file.html)               | Build a proofread rule diagnostic for a file.                  |
| [`make-suggesting`](proofread.rules.diagnostics/make-suggesting.html)   | Build a proofread rule diagnostic with structured suggestions. |
| [`rewrite`](proofread.rules.diagnostics/rewrite.html)                   | Build a single-edit rewrite suggestion.                        |
| [`with-suggestions`](proofread.rules.diagnostics/with-suggestions.html) | Attach suggestions to a diagnostic.                            |

</main>
</div>
