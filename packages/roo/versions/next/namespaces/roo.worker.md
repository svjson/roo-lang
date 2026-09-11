---
layout: reference
title: roo.worker
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roo.worker.html">roo.worker</a>
<ol>
<li><a href="roo.worker/collect-bang.html">collect!</a></li>
<li><a href="roo.worker/create-bang.html">create!</a></li>
<li><a href="roo.worker/execute-let-bang.html">execute-let!</a></li>
<li><a href="roo.worker/invoke-bang.html">invoke!</a></li>
<li><a href="roo.worker/poll-bang.html">poll!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roo.worker</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roo.worker.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roo.worker

| Symbol                                             | Summary                                                         |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [`collect!`](roo.worker/collect-bang.html)         | Consume and return a completed worker execution's result.       |
| [`create!`](roo.worker/create-bang.html)           | Create and start a named worker owned by the current runtime.   |
| [`execute-let!`](roo.worker/execute-let-bang.html) | Schedule worker-side evaluation with parent-evaluated bindings. |
| [`invoke!`](roo.worker/invoke-bang.html)           | Schedule a callable invocation in a named worker runtime.       |
| [`poll!`](roo.worker/poll-bang.html)               | Return the current status of a worker execution.                |

</main>
</div>
