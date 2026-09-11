---
layout: reference
title: footsteps.progress
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="footsteps.progress.html">footsteps.progress</a>
<ol>
<li><a href="footsteps.progress/clamp.html">clamp</a></li>
<li><a href="footsteps.progress/project.html">project</a></li>
<li><a href="footsteps.progress/projected-value.html">projected-value</a></li>
<li><a href="footsteps.progress/ratio.html">ratio</a></li>
<li><a href="footsteps.progress/raw-value.html">raw-value</a></li>
<li><a href="footsteps.progress/report.html">report</a></li>
<li><a href="footsteps.progress/value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps.progress</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/footsteps.progress.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps.progress

| Symbol                                                       | Summary                                                 |
| ------------------------------------------------------------ | ------------------------------------------------------- |
| [`clamp`](footsteps.progress/clamp.html)                     |                                                         |
| [`project`](footsteps.progress/project.html)                 | Project child progress into a parent progress scope.    |
| [`projected-value`](footsteps.progress/projected-value.html) | Return the global progress value represented by a step. |
| [`ratio`](footsteps.progress/ratio.html)                     | Return a clamped progress ratio for a report.           |
| [`raw-value`](footsteps.progress/raw-value.html)             |                                                         |
| [`report`](footsteps.progress/report.html)                   | Normalize a progress report candidate.                  |
| [`value`](footsteps.progress/value.html)                     | Return the current progress report for a work value.    |

</main>
</div>
