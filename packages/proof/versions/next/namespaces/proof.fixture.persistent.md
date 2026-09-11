---
layout: reference
title: proof.fixture.persistent
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.fixture.persistent.html">proof.fixture.persistent</a>
<ol>
<li><a href="proof.fixture.persistent/configure-bang.html">configure!</a></li>
<li><a href="proof.fixture.persistent/fixture-format.html">fixture-format</a></li>
<li><a href="proof.fixture.persistent/fixture-path.html">fixture-path</a></li>
<li><a href="proof.fixture.persistent/fixture-root.html">fixture-root</a></li>
<li><a href="proof.fixture.persistent/generated-value-bang.html">generated-value!</a></li>
<li><a href="proof.fixture.persistent/value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.fixture.persistent</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.fixture.persistent.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.fixture.persistent

| Symbol                                                                   | Summary                                                                     |
| ------------------------------------------------------------------------ | --------------------------------------------------------------------------- |
| [`configure!`](proof.fixture.persistent/configure-bang.html)             | Configures persistent fixture storage.                                      |
| [`fixture-format`](proof.fixture.persistent/fixture-format.html)         | EDN format marker for proof persistent fixture documents.                   |
| [`fixture-path`](proof.fixture.persistent/fixture-path.html)             | Returns the storage path for `relative-path` under the active fixture root. |
| [`fixture-root`](proof.fixture.persistent/fixture-root.html)             | Root directory for persistent fixture documents.                            |
| [`generated-value!`](proof.fixture.persistent/generated-value-bang.html) | Generates, projects, writes, and returns a persistent fixture value.        |
| [`value`](proof.fixture.persistent/value.html)                           | Returns a persistent fixture value.                                         |

</main>
</div>
