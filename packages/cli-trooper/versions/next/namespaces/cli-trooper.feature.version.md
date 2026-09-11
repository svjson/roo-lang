---
layout: reference
title: cli-trooper.feature.version
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.feature.version.html">cli-trooper.feature.version</a>
<ol>
<li><a href="cli-trooper.feature.version/lines.html">lines</a></li>
<li><a href="cli-trooper.feature.version/run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.feature.version</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.feature.version.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.feature.version

| Symbol                                              | Summary                                                    |
| --------------------------------------------------- | ---------------------------------------------------------- |
| [`lines`](cli-trooper.feature.version/lines.html)   | Build output for the attached application version command. |
| [`run!`](cli-trooper.feature.version/run-bang.html) | Print output for a command-shaped version invocation.      |

</main>
</div>
