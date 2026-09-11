---
layout: reference
title: roo.string/replace
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.string.html">roo.string</a>
<ol>
<li><a href="ends-with-p.html">ends-with?</a></li>
<li><a class="active" href="replace.html">replace</a></li>
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
<span aria-current="page">replace</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.string/replace.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.string.html">roo.string</a>/</div>
<h1><code>replace</code></h1>
</header>

Replace all literal substring occurrences in a string.

<hr class="signature-divider">

```roo
(replace source match replacement)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.string/replace "Normal Club" "Normal" "Enchanted")
=> "Enchanted Club"
```

<hr class="signature-divider">

## Signature:

```roo
(replace source match replacement)
```

| Arg         | Description                       |
| ----------- | --------------------------------- |
| source      | The source string.                |
| match       | The literal substring to replace. |
| replacement | The replacement string.           |

</main>
</div>
