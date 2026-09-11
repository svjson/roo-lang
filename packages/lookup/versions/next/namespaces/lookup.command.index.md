---
layout: reference
title: lookup.command.index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.command.index.html">lookup.command.index</a>
<ol>
<li><a href="lookup.command.index/command.html">command</a></li>
<li><a href="lookup.command.index/execute-bang.html">execute!</a></li>
<li><a href="lookup.command.index/validation-errors.html">validation-errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.command.index</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.command.index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.command.index

| Symbol                                                             | Summary                                                                  |
| ------------------------------------------------------------------ | ------------------------------------------------------------------------ |
| [`command`](lookup.command.index/command.html)                     | cli-trooper declaration for the `lookup index` command.                  |
| [`execute!`](lookup.command.index/execute-bang.html)               | Build an index artifact and print or write the requested representation. |
| [`validation-errors`](lookup.command.index/validation-errors.html) | Validate semantic constraints between lookup index options.              |

</main>
</div>
