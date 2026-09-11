---
layout: reference
title: cli-trooper.config
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.config.html">cli-trooper.config</a>
<ol>
<li><a href="cli-trooper.config/normalize.html">normalize</a></li>
<li><a href="cli-trooper.config/normalize-result.html">normalize-result</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.config</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.config.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.config

| Symbol                                                         | Summary                                                                   |
| -------------------------------------------------------------- | ------------------------------------------------------------------------- |
| [`normalize`](cli-trooper.config/normalize.html)               | Normalize ergonomic CLI configuration into the canonical public model.    |
| [`normalize-result`](cli-trooper.config/normalize-result.html) | Normalize configuration and collect structured configuration diagnostics. |

</main>
</div>
