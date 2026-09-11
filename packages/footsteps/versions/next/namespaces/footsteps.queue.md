---
layout: reference
title: footsteps.queue
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="footsteps.queue.html">footsteps.queue</a>
<ol>
<li><a href="footsteps.queue/allocate-instances.html">allocate-instances</a></li>
<li><a href="footsteps.queue/append-after-current.html">append-after-current</a></li>
<li><a href="footsteps.queue/append-hidden.html">append-hidden</a></li>
<li><a href="footsteps.queue/append-strategy.html">append-strategy</a></li>
<li><a href="footsteps.queue/append-within-current-scope.html">append-within-current-scope</a></li>
<li><a href="footsteps.queue/append-work.html">append-work</a></li>
<li><a href="footsteps.queue/instance.html">instance</a></li>
<li><a href="footsteps.queue/step-weight.html">step-weight</a></li>
<li><a href="footsteps.queue/total-weight.html">total-weight</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps.queue</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/footsteps.queue.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps.queue

| Symbol                                                                            | Summary                                                        |
| --------------------------------------------------------------------------------- | -------------------------------------------------------------- |
| [`allocate-instances`](footsteps.queue/allocate-instances.html)                   | Allocate queued step instances across a parent progress scope. |
| [`append-after-current`](footsteps.queue/append-after-current.html)               |                                                                |
| [`append-hidden`](footsteps.queue/append-hidden.html)                             |                                                                |
| [`append-strategy`](footsteps.queue/append-strategy.html)                         | Append strategy keywords accepted by step results.             |
| [`append-within-current-scope`](footsteps.queue/append-within-current-scope.html) |                                                                |
| [`append-work`](footsteps.queue/append-work.html)                                 | Append dynamic work returned by a completed step.              |
| [`instance`](footsteps.queue/instance.html)                                       |                                                                |
| [`step-weight`](footsteps.queue/step-weight.html)                                 | Return the progress weight for a queued step entry.            |
| [`total-weight`](footsteps.queue/total-weight.html)                               |                                                                |

</main>
</div>
