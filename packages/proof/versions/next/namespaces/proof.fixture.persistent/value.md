---
layout: reference
title: proof.fixture.persistent/value
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.fixture.persistent.html">proof.fixture.persistent</a>
<ol>
<li><a href="configure-bang.html">configure!</a></li>
<li><a href="fixture-format.html">fixture-format</a></li>
<li><a href="fixture-path.html">fixture-path</a></li>
<li><a href="fixture-root.html">fixture-root</a></li>
<li><a href="generated-value-bang.html">generated-value!</a></li>
<li><a class="active" href="value.html">value</a></li>
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
<span aria-current="page">value</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.fixture.persistent/value.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.fixture.persistent.html">proof.fixture.persistent</a>/</div>
<h1><code>value</code></h1>
</header>

Returns a persistent fixture value.

<hr class="signature-divider">

```roo
(value relative-path generate & opts &rest)
```

<hr class="signature-divider">

Reads the fixture document when it exists and matches `:version`. Otherwise,
calls `generate`, writes the resulting document, and returns the generated
value. Set `:refresh? true` in `opts` to force regeneration.

<hr class="signature-divider">

## Signature:

```roo
(value relative-path generate & opts &rest)
```

| Arg           | Description |
| ------------- | ----------- |
| relative-path |             |
| generate      |             |
| opts          |             |
| &rest         |             |

</main>
</div>
