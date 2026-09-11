---
layout: reference
title: lookup.cli
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.cli.html">lookup.cli</a>
<ol>
<li><a href="lookup.cli/application.html">application</a></li>
<li><a href="lookup.cli/main.html">main</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.cli</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.cli.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.cli

| Symbol                                       | Summary                                                          |
| -------------------------------------------- | ---------------------------------------------------------------- |
| [`application`](lookup.cli/application.html) | cli-trooper declaration for the Lookup command-line application. |
| [`main`](lookup.cli/main.html)               | Run the Lookup command-line application.                         |

</main>
</div>
