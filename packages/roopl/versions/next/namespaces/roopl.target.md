---
layout: reference
title: roopl.target
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roopl.target.html">roopl.target</a>
<ol>
<li><a href="roopl.target/close-bang.html">close!</a></li>
<li><a href="roopl.target/eval-bang.html">eval!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roopl.target</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roopl.target.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roopl.target

| Symbol                                   | Summary                               |
| ---------------------------------------- | ------------------------------------- |
| [`close!`](roopl.target/close-bang.html) | Release TARGET's execution resources. |
| [`eval!`](roopl.target/eval-bang.html)   | Evaluate one request through TARGET.  |

</main>
</div>
