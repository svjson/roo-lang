---
layout: reference
title: footsteps next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps next</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps next

Cooperative resumable work and progress accounting for Roo programs.

| Namespace                                                | Symbols |
| -------------------------------------------------------- | ------- |
| [footsteps](namespaces/footsteps.html)                   | 10      |
| [footsteps.estimate](namespaces/footsteps.estimate.html) | 2       |
| [footsteps.progress](namespaces/footsteps.progress.html) | 7       |
| [footsteps.queue](namespaces/footsteps.queue.html)       | 9       |
| [footsteps.scope](namespaces/footsteps.scope.html)       | 3       |
| [footsteps.work](namespaces/footsteps.work.html)         | 14      |
