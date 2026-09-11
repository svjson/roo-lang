---
layout: reference
title: cli-trooper
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.html">cli-trooper</a>
<ol>
<li><a href="cli-trooper/parse.html">parse</a></li>
<li><a href="cli-trooper/prepare.html">prepare</a></li>
<li><a href="cli-trooper/run.html">run</a></li>
<li><a href="cli-trooper/run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper

| Symbol                                | Summary                                                                        |
| ------------------------------------- | ------------------------------------------------------------------------------ |
| [`parse`](cli-trooper/parse.html)     | Normalize configuration and parse arguments without executing a command.       |
| [`prepare`](cli-trooper/prepare.html) | Normalize configuration and parse invocation arguments for public entrypoints. |
| [`run`](cli-trooper/run.html)         | Normalize configuration, parse arguments, and invoke the selected command.     |
| [`run!`](cli-trooper/run-bang.html)   | Run a declared CLI command and return a process-compatible exit code.          |

</main>
</div>
