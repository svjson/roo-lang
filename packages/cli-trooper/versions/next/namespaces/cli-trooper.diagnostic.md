---
layout: reference
title: cli-trooper.diagnostic
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<ol>
<li><a href="cli-trooper.diagnostic/expected-text.html">expected-text</a></li>
<li><a href="cli-trooper.diagnostic/line.html">line</a></li>
<li><a href="cli-trooper.diagnostic/lines.html">lines</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.diagnostic</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.diagnostic.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.diagnostic

| Symbol                                                       | Summary                                                        |
| ------------------------------------------------------------ | -------------------------------------------------------------- |
| [`expected-text`](cli-trooper.diagnostic/expected-text.html) | Format an expected diagnostic value for human-readable output. |
| [`line`](cli-trooper.diagnostic/line.html)                   | Render one structured cli-trooper diagnostic as text.          |
| [`lines`](cli-trooper.diagnostic/lines.html)                 | Render structured cli-trooper diagnostics as text.             |

</main>
</div>
