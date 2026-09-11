---
layout: reference
title: cli-trooper.parse.command
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<ol>
<li><a href="cli-trooper.parse.command/command-flag.html">command-flag</a></li>
<li><a href="cli-trooper.parse.command/pending-defaults.html">pending-defaults</a></li>
<li><a href="cli-trooper.parse.command/select.html">select</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.parse.command</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.parse.command.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.parse.command

| Symbol                                                                | Summary                                                                        |
| --------------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| [`command-flag`](cli-trooper.parse.command/command-flag.html)         | Find a command-shaped flag declaration matching one argument.                  |
| [`pending-defaults`](cli-trooper.parse.command/pending-defaults.html) | Collect one command's unresolved canonical defaults.                           |
| [`select`](cli-trooper.parse.command/select.html)                     | Resolve positional or command-shaped flag selection from an argument sequence. |

</main>
</div>
