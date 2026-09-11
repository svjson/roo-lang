---
layout: reference
title: proofread.rules
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.html">proofread.rules</a>
<ol>
<li><a href="proofread.rules/categories.html">categories</a></li>
<li><a href="proofread.rules/check.html">check</a></li>
<li><a href="proofread.rules/check-file-with-config.html">check-file-with-config</a></li>
<li><a href="proofread.rules/check-with-config.html">check-with-config</a></li>
<li><a href="proofread.rules/enabled-rules.html">enabled-rules</a></li>
<li><a href="proofread.rules/rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules

| Symbol                                                                  | Summary                                                     |
| ----------------------------------------------------------------------- | ----------------------------------------------------------- |
| [`categories`](proofread.rules/categories.html)                         | Ordered module maps for proofread rule category namespaces. |
| [`check`](proofread.rules/check.html)                                   | Run all proofread rules for parsed source roots.            |
| [`check-file-with-config`](proofread.rules/check-file-with-config.html) | Run enabled file-level proofread rules.                     |
| [`check-with-config`](proofread.rules/check-with-config.html)           | Run enabled proofread rules for parsed source roots.        |
| [`enabled-rules`](proofread.rules/enabled-rules.html)                   | Return rule descriptors enabled by config.                  |
| [`rules`](proofread.rules/rules.html)                                   | Registered proofread rule descriptors in category order.    |

</main>
</div>
