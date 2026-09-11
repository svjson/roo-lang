---
layout: reference
title: spool.report
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="spool.report.html">spool.report</a>
<ol>
<li><a href="spool.report/append-context-lines-bang.html">append-context-lines!</a></li>
<li><a href="spool.report/lines.html">lines</a></li>
<li><a href="spool.report/project-context.html">project-context</a></li>
<li><a href="spool.report/project-contexts.html">project-contexts</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">spool.report</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/spool.report.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# spool.report

| Symbol                                                                 | Summary                                                         |
| ---------------------------------------------------------------------- | --------------------------------------------------------------- |
| [`append-context-lines!`](spool.report/append-context-lines-bang.html) | Append rendered context lines depth-first.                      |
| [`lines`](spool.report/lines.html)                                     | Render a report tree as text lines.                             |
| [`project-context`](spool.report/project-context.html)                 | Apply report selection and collapsing options to one context.   |
| [`project-contexts`](spool.report/project-contexts.html)               | Apply report selection and collapsing options to context nodes. |

</main>
</div>
