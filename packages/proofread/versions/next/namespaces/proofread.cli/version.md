---
layout: reference
title: proofread.cli/version
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.cli.html">proofread.cli</a>
<ol>
<li><a href="application.html">application</a></li>
<li><a href="main.html">main</a></li>
<li><a class="active" href="version.html">version</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.cli.html">proofread.cli</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">version</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.cli/version.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.cli.html">proofread.cli</a>/</div>
<h1><code>version</code></h1>
</header>

Proofread command version.





</main>
</div>
