---
layout: reference
title: proof.cli
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.cli.html">proof.cli</a>
<ol>
<li><a href="proof.cli/application.html">application</a></li>
<li><a href="proof.cli/main.html">main</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.cli</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.cli.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.cli

| Symbol                                      | Summary                                               |
| ------------------------------------------- | ----------------------------------------------------- |
| [`application`](proof.cli/application.html) | Proof's supplied cli-trooper application declaration. |
| [`main`](proof.cli/main.html)               | Run Proof through its direct command-line entrypoint. |

</main>
</div>
