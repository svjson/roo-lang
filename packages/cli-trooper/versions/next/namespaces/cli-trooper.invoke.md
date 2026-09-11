---
layout: reference
title: cli-trooper.invoke
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.invoke.html">cli-trooper.invoke</a>
<ol>
<li><a href="cli-trooper.invoke/command-flag-execution.html">command-flag-execution</a></li>
<li><a href="cli-trooper.invoke/resolve-execution.html">resolve-execution</a></li>
<li><a href="cli-trooper.invoke/run.html">run</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.invoke</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.invoke.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.invoke

| Symbol                                                                     | Summary                                                               |
| -------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`command-flag-execution`](cli-trooper.invoke/command-flag-execution.html) | Resolve a command-shaped flag execution function by command identity. |
| [`resolve-execution`](cli-trooper.invoke/resolve-execution.html)           | Resolve the execution function for a selected command identity.       |
| [`run`](cli-trooper.invoke/run.html)                                       | Invoke the execution function selected by a successful parse result.  |

</main>
</div>
