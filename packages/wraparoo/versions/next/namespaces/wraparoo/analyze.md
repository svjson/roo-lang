---
layout: reference
title: wraparoo/analyze
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.html">wraparoo</a>
<ol>
<li><a class="active" href="analyze.html">analyze</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.html">wraparoo</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">analyze</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo/analyze.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.html">wraparoo</a>/</div>
<h1><code>analyze</code></h1>
</header>

Analyze complete or incomplete Roo SOURCE without evaluating or repairing it.

<hr class="signature-divider">

```roo
(analyze source)
```

<hr class="signature-divider">

## Signature:

```roo
(analyze source)
```

| Arg    | Description                   |
| ------ | ----------------------------- |
| source | Roo source string to analyze. |

### Returns:

A map containing lossless `:tokens`, neutral `:syntax-ranges`, structurally
valid `:delimiter-pairs`, and source-ordered `:unmatched-delimiters`. Every
range is a half-open Unicode-scalar range into SOURCE.

```roo
{:tokens [...]
:syntax-ranges [...]
:delimiter-pairs [...]
:unmatched-delimiters [...]}
```

</main>
</div>
