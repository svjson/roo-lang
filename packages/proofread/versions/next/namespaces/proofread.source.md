---
layout: reference
title: proofread.source
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.source.html">proofread.source</a>
<ol>
<li><a href="proofread.source/context.html">context</a></li>
<li><a href="proofread.source/diagnostics.html">diagnostics</a></li>
<li><a href="proofread.source/diagnostics-with-config.html">diagnostics-with-config</a></li>
<li><a href="proofread.source/namespace-name.html">namespace-name</a></li>
<li><a href="proofread.source/package-name.html">package-name</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.source</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.source.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.source

| Symbol                                                                     | Summary                                                              |
| -------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| [`context`](proofread.source/context.html)                                 | Build file-level context for reporter grouping.                      |
| [`diagnostics`](proofread.source/diagnostics.html)                         | Run proofread rules using the discovered config for a file.          |
| [`diagnostics-with-config`](proofread.source/diagnostics-with-config.html) | Run proofread rules and attach source context for reporter grouping. |
| [`namespace-name`](proofread.source/namespace-name.html)                   | Return the namespace declared by parsed Roo source roots.            |
| [`package-name`](proofread.source/package-name.html)                       | Return the package name for a source file.                           |

</main>
</div>
