---
layout: reference
title: boodle.cli
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.cli.html">boodle.cli</a>
<ol>
<li><a href="boodle.cli/application.html">application</a></li>
<li><a href="boodle.cli/main.html">main</a></li>
<li><a href="boodle.cli/version.html">version</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.cli</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.cli.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.cli

| Symbol                                       | Summary                                                |
| -------------------------------------------- | ------------------------------------------------------ |
| [`application`](boodle.cli/application.html) | Boodle's supplied cli-trooper application declaration. |
| [`main`](boodle.cli/main.html)               | Run the Boodle command line entry point.               |
| [`version`](boodle.cli/version.html)         | Boodle command version.                                |

</main>
</div>
