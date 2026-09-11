---
layout: reference
title: footsteps
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="footsteps.html">footsteps</a>
<ol>
<li><a href="footsteps/advance.html">advance</a></li>
<li><a href="footsteps/append-strategy.html">append-strategy</a></li>
<li><a href="footsteps/complete-p.html">complete?</a></li>
<li><a href="footsteps/estimate.html">estimate</a></li>
<li><a href="footsteps/make.html">make</a></li>
<li><a href="footsteps/progress.html">progress</a></li>
<li><a href="footsteps/progress-report.html">progress-report</a></li>
<li><a href="footsteps/project-progress.html">project-progress</a></li>
<li><a href="footsteps/run.html">run</a></li>
<li><a href="footsteps/slice.html">slice</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/footsteps.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps

| Symbol                                                | Summary                                                  |
| ----------------------------------------------------- | -------------------------------------------------------- |
| [`advance`](footsteps/advance.html)                   | Advance a work value by one observable step.             |
| [`append-strategy`](footsteps/append-strategy.html)   | Append strategy keywords accepted by work step results.  |
| [`complete?`](footsteps/complete-p.html)              | Return whether a work value has completed.               |
| [`estimate`](footsteps/estimate.html)                 | Resolve a progress estimate value.                       |
| [`make`](footsteps/make.html)                         | Create a footsteps work value from a work specification. |
| [`progress`](footsteps/progress.html)                 | Return the current progress report for a work value.     |
| [`progress-report`](footsteps/progress-report.html)   | Normalize a progress report candidate.                   |
| [`project-progress`](footsteps/project-progress.html) | Project child progress into a parent progress scope.     |
| [`run`](footsteps/run.html)                           | Advance work repeatedly inside a time and step budget.   |
| [`slice`](footsteps/slice.html)                       | Advance work for one cooperative time slice.             |

</main>
</div>
