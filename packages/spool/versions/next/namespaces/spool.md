---
layout: reference
title: spool
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="spool.html">spool</a>
<ol>
<li><a href="spool/enter-bang.html">enter!</a></li>
<li><a href="spool/leave-bang.html">leave!</a></li>
<li><a href="spool/record-bang.html">record!</a></li>
<li><a href="spool/report-bang.html">report!</a></li>
<li><a href="spool/reset-bang.html">reset!</a></li>
<li><a href="spool/wrap.html">wrap</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">spool</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/spool.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# spool

| Symbol                              | Summary                                                          |
| ----------------------------------- | ---------------------------------------------------------------- |
| [`enter!`](spool/enter-bang.html)   | Open a timed context.                                            |
| [`leave!`](spool/leave-bang.html)   | Close the current timed context when its key matches.            |
| [`record!`](spool/record-bang.html) | Record a map of metadata, durable until the next `reset!`        |
| [`report!`](spool/report-bang.html) | Return the collected context tree and optionally print it.       |
| [`reset!`](spool/reset-bang.html)   | Discard every recorded and open context.                         |
| [`wrap`](spool/wrap.html)           | Return a function that records each call inside a timed context. |

</main>
</div>
