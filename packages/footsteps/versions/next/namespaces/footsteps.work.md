---
layout: reference
title: footsteps.work
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="footsteps.work.html">footsteps.work</a>
<ol>
<li><a href="footsteps.work/advance.html">advance</a></li>
<li><a href="footsteps.work/apply-delegated-work.html">apply-delegated-work</a></li>
<li><a href="footsteps.work/apply-step-result.html">apply-step-result</a></li>
<li><a href="footsteps.work/complete-current.html">complete-current</a></li>
<li><a href="footsteps.work/complete-p.html">complete?</a></li>
<li><a href="footsteps.work/completed-if-drained.html">completed-if-drained</a></li>
<li><a href="footsteps.work/continue-delegated-work.html">continue-delegated-work</a></li>
<li><a href="footsteps.work/default-work-slice-limit.html">default-work-slice-limit</a></li>
<li><a href="footsteps.work/finish-current.html">finish-current</a></li>
<li><a href="footsteps.work/make.html">make</a></li>
<li><a href="footsteps.work/run.html">run</a></li>
<li><a href="footsteps.work/slice.html">slice</a></li>
<li><a href="footsteps.work/start-current.html">start-current</a></li>
<li><a href="footsteps.work/with-delegated-work.html">with-delegated-work</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps.work</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/footsteps.work.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps.work

| Symbol                                                                     | Summary                                                          |
| -------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`advance`](footsteps.work/advance.html)                                   | Advance a work value by one observable step.                     |
| [`apply-delegated-work`](footsteps.work/apply-delegated-work.html)         |                                                                  |
| [`apply-step-result`](footsteps.work/apply-step-result.html)               |                                                                  |
| [`complete-current`](footsteps.work/complete-current.html)                 |                                                                  |
| [`complete?`](footsteps.work/complete-p.html)                              | Return whether a work value has completed.                       |
| [`completed-if-drained`](footsteps.work/completed-if-drained.html)         |                                                                  |
| [`continue-delegated-work`](footsteps.work/continue-delegated-work.html)   |                                                                  |
| [`default-work-slice-limit`](footsteps.work/default-work-slice-limit.html) | Default maximum number of steps `slice` may advance in one call. |
| [`finish-current`](footsteps.work/finish-current.html)                     |                                                                  |
| [`make`](footsteps.work/make.html)                                         | Create a footsteps work value from a work specification.         |
| [`run`](footsteps.work/run.html)                                           | Advance work repeatedly inside a time and step budget.           |
| [`slice`](footsteps.work/slice.html)                                       | Advance work for one cooperative time slice.                     |
| [`start-current`](footsteps.work/start-current.html)                       |                                                                  |
| [`with-delegated-work`](footsteps.work/with-delegated-work.html)           |                                                                  |

</main>
</div>
