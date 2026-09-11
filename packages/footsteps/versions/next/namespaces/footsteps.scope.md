---
layout: reference
title: footsteps.scope
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="footsteps.scope.html">footsteps.scope</a>
<ol>
<li><a href="footsteps.scope/default-progress-max.html">default-progress-max</a></li>
<li><a href="footsteps.scope/make.html">make</a></li>
<li><a href="footsteps.scope/size.html">size</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">footsteps next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">footsteps.scope</span>
</nav>
<div class="version-context">
<span class="version-package">footsteps</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["footsteps"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/footsteps.scope.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# footsteps.scope

| Symbol                                                              | Summary                                                             |
| ------------------------------------------------------------------- | ------------------------------------------------------------------- |
| [`default-progress-max`](footsteps.scope/default-progress-max.html) | Default progress scale used when a work spec omits `:progress-max`. |
| [`make`](footsteps.scope/make.html)                                 | Create a progress scope.                                            |
| [`size`](footsteps.scope/size.html)                                 | Return the non-negative size of a progress scope.                   |

</main>
</div>
