---
layout: reference
title: roo.string/ends-with?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.string.html">roo.string</a>
<ol>
<li><a class="active" href="ends-with-p.html">ends-with?</a></li>
<li><a href="replace.html">replace</a></li>
<li><a href="replace-first.html">replace-first</a></li>
<li><a href="starts-with-p.html">starts-with?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.string.html">roo.string</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">ends-with?</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.string/ends-with-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.string.html">roo.string</a>/</div>
<h1><code>ends-with?</code></h1>
</header>

Tests whether a string ends with a suffix.

<hr class="signature-divider">

```roo
(ends-with? source suffix)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.string/ends-with? "abcdef" "def") => true
(roo.string/ends-with? "abcdef" "bcd") => false
```

<hr class="signature-divider">

## Signature:

```roo
(ends-with? source suffix)
```

| Arg    | Description          |
| ------ | -------------------- |
| source | The source string.   |
| suffix | The suffix to match. |

</main>
</div>
