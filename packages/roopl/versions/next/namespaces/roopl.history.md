---
layout: reference
title: roopl.history
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roopl.history.html">roopl.history</a>
<ol>
<li><a href="roopl.history/default-path-bang.html">default-path!</a></li>
<li><a href="roopl.history/load-bang.html">load!</a></li>
<li><a href="roopl.history/max-entries.html">max-entries</a></li>
<li><a href="roopl.history/record-bang.html">record!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roopl.history</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roopl.history.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roopl.history

| Symbol                                                  | Summary                                                                |
| ------------------------------------------------------- | ---------------------------------------------------------------------- |
| [`default-path!`](roopl.history/default-path-bang.html) | Return the default per-user roopl history path.                        |
| [`load!`](roopl.history/load-bang.html)                 | Load recent nonblank commands from PATH.                               |
| [`max-entries`](roopl.history/max-entries.html)         | Maximum number of commands retained in memory and in `.roopl_history`. |
| [`record!`](roopl.history/record-bang.html)             | Record COMMAND and atomically persist recent history to PATH.          |

</main>
</div>
