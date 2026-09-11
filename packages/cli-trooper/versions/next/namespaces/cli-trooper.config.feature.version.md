---
layout: reference
title: cli-trooper.config.feature.version
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.config.feature.version.html">cli-trooper.config.feature.version</a>
<ol>
<li><a href="cli-trooper.config.feature.version/apply.html">apply</a></li>
<li><a href="cli-trooper.config.feature.version/errors.html">errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.config.feature.version</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.config.feature.version.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.config.feature.version

| Symbol                                                     | Summary                                                              |
| ---------------------------------------------------------- | -------------------------------------------------------------------- |
| [`apply`](cli-trooper.config.feature.version/apply.html)   | Attach the configured version command flag to a canonical CLI model. |
| [`errors`](cli-trooper.config.feature.version/errors.html) | Validate explicit use of the default version feature.                |

</main>
</div>
