---
layout: reference
title: proof.fixture.persistent/fixture-path
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.fixture.persistent.html">proof.fixture.persistent</a>
<ol>
<li><a href="configure-bang.html">configure!</a></li>
<li><a href="fixture-format.html">fixture-format</a></li>
<li><a class="active" href="fixture-path.html">fixture-path</a></li>
<li><a href="fixture-root.html">fixture-root</a></li>
<li><a href="generated-value-bang.html">generated-value!</a></li>
<li><a href="value.html">value</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.fixture.persistent.html">proof.fixture.persistent</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">fixture-path</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.fixture.persistent/fixture-path.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.fixture.persistent.html">proof.fixture.persistent</a>/</div>
<h1><code>fixture-path</code></h1>
</header>

Returns the storage path for `relative-path` under the active fixture root.

<hr class="signature-divider">

```roo
(fixture-path relative-path)
```

<hr class="signature-divider">

## Signature:

```roo
(fixture-path relative-path)
```

| Arg           | Description |
| ------------- | ----------- |
| relative-path |             |

</main>
</div>
