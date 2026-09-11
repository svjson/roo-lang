---
layout: reference
title: proofread.cli
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.cli.html">proofread.cli</a>
<ol>
<li><a href="proofread.cli/application.html">application</a></li>
<li><a href="proofread.cli/main.html">main</a></li>
<li><a href="proofread.cli/version.html">version</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.cli</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.cli.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.cli

| Symbol                                          | Summary                                                   |
| ----------------------------------------------- | --------------------------------------------------------- |
| [`application`](proofread.cli/application.html) | Proofread's supplied cli-trooper application declaration. |
| [`main`](proofread.cli/main.html)               | Run the proofread command line entry point.               |
| [`version`](proofread.cli/version.html)         | Proofread command version.                                |

</main>
</div>
