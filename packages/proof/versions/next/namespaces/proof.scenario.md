---
layout: reference
title: proof.scenario
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.scenario.html">proof.scenario</a>
<ol>
<li><a href="proof.scenario/given.html">given</a></li>
<li><a href="proof.scenario/then.html">then</a></li>
<li><a href="proof.scenario/when.html">when</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.scenario</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.scenario.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.scenario

| Symbol                               | Summary                                          |
| ------------------------------------ | ------------------------------------------------ |
| [`given`](proof.scenario/given.html) | Seeds the current state of a test scenario.      |
| [`then`](proof.scenario/then.html)   | Observes the current state of a test scenario.   |
| [`when`](proof.scenario/when.html)   | Transforms the current state of a test scenario. |

</main>
</div>
