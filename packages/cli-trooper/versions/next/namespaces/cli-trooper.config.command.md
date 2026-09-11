---
layout: reference
title: cli-trooper.config.command
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.config.command.html">cli-trooper.config.command</a>
<ol>
<li><a href="cli-trooper.config.command/mixed-flags.html">mixed-flags</a></li>
<li><a href="cli-trooper.config.command/normalize.html">normalize</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.config.command</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.config.command.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.config.command

| Symbol                                                       | Summary                                                                |
| ------------------------------------------------------------ | ---------------------------------------------------------------------- |
| [`mixed-flags`](cli-trooper.config.command/mixed-flags.html) | Compose inherited and command-local flag declarations.                 |
| [`normalize`](cli-trooper.config.command/normalize.html)     | Normalize one command while retaining unrecognized prototype metadata. |

</main>
</div>
