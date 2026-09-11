---
layout: reference
title: proof.fixture.cache/clear!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.fixture.cache.html">proof.fixture.cache</a>
<ol>
<li><a class="active" href="clear-bang.html">clear!</a></li>
<li><a href="entries.html">entries</a></li>
<li><a href="value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.fixture.cache.html">proof.fixture.cache</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">clear!</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.fixture.cache/clear-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.fixture.cache.html">proof.fixture.cache</a>/</div>
<h1><code>clear!</code></h1>
</header>

Clears all in-process cache fixture values.

<hr class="signature-divider">

```roo
(clear!)
```

<hr class="signature-divider">

Returns the empty fixture cache.

<hr class="signature-divider">

## Signature:

```roo
(clear!)
```

</main>
</div>
