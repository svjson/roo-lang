---
layout: reference
title: proof.fixture.cache/value
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.fixture.cache.html">proof.fixture.cache</a>
<ol>
<li><a href="clear-bang.html">clear!</a></li>
<li><a href="entries.html">entries</a></li>
<li><a class="active" href="value.html">value</a></li>
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
<span aria-current="page">value</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.fixture.cache/value.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.fixture.cache.html">proof.fixture.cache</a>/</div>
<h1><code>value</code></h1>
</header>

Returns an in-process cache fixture value.

<hr class="signature-divider">

```roo
(value key generate & opts &rest)
```

<hr class="signature-divider">

`key` identifies the value for the current proof process. `generate` is called
only when the key is absent. `opts` may contain `:paths` to retain only stable
nested paths from the generated value.

<hr class="signature-divider">

## Signature:

```roo
(value key generate & opts &rest)
```

| Arg      | Description |
| -------- | ----------- |
| key      |             |
| generate |             |
| opts     |             |
| &rest    |             |

</main>
</div>
