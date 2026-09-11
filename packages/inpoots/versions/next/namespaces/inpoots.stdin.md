---
layout: reference
title: inpoots.stdin
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="inpoots.stdin.html">inpoots.stdin</a>
<ol>
<li><a href="inpoots.stdin/read-event-bang.html">read-event!</a></li>
<li><a href="inpoots.stdin/with-bang.html">with!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">inpoots next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">inpoots.stdin</span>
</nav>
<div class="version-context">
<span class="version-package">inpoots</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["inpoots"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/inpoots.stdin.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# inpoots.stdin

| Symbol                                              | Summary                                             |
| --------------------------------------------------- | --------------------------------------------------- |
| [`read-event!`](inpoots.stdin/read-event-bang.html) | Block until INPUT yields a portable terminal event. |
| [`with!`](inpoots.stdin/with-bang.html)             | Run CALLBACK with scoped access to standard input.  |

</main>
</div>
