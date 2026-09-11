---
layout: reference
title: proofread.inspect.namespaces
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>
<ol>
<li><a href="proofread.inspect.namespaces/directory-entry-leaves.html">directory-entry-leaves</a></li>
<li><a href="proofread.inspect.namespaces/directory-entry-source-p.html">directory-entry-source?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.namespaces</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.namespaces.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.namespaces

| Symbol                                                                                  | Summary                                                              |
| --------------------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| [`directory-entry-leaves`](proofread.inspect.namespaces/directory-entry-leaves.html)    | Return the namespace leaves valid for a directory entry source file. |
| [`directory-entry-source?`](proofread.inspect.namespaces/directory-entry-source-p.html) | Test whether a source path is a namespace directory entry file.      |

</main>
</div>
