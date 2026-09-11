---
layout: reference
title: proof.fixture.cache
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.fixture.cache.html">proof.fixture.cache</a>
<ol>
<li><a href="proof.fixture.cache/clear-bang.html">clear!</a></li>
<li><a href="proof.fixture.cache/entries.html">entries</a></li>
<li><a href="proof.fixture.cache/value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.fixture.cache</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.fixture.cache.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.fixture.cache

| Symbol                                          | Summary                                                       |
| ----------------------------------------------- | ------------------------------------------------------------- |
| [`clear!`](proof.fixture.cache/clear-bang.html) | Clears all in-process cache fixture values.                   |
| [`entries`](proof.fixture.cache/entries.html)   | In-process fixture values keyed by user-supplied fixture key. |
| [`value`](proof.fixture.cache/value.html)       | Returns an in-process cache fixture value.                    |

</main>
</div>
