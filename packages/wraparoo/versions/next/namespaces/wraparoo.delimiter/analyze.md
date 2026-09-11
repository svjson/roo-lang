---
layout: reference
title: wraparoo.delimiter/analyze
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.delimiter.html">wraparoo.delimiter</a>
<ol>
<li><a class="active" href="analyze.html">analyze</a></li>
<li><a href="pair-at.html">pair-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.delimiter.html">wraparoo.delimiter</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">analyze</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.delimiter/analyze.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.delimiter.html">wraparoo.delimiter</a>/</div>
<h1><code>analyze</code></h1>
</header>

Find structurally valid delimiter pairs and unmatched delimiters in TOKENS.

<hr class="signature-divider">

```roo
(analyze tokens)
```

<hr class="signature-divider">

## Signature:

```roo
(analyze tokens)
```

| Arg    | Description                                            |
| ------ | ------------------------------------------------------ |
| tokens | Lossless token vector returned by `wraparoo.lex/scan`. |

### Returns:

A map containing `:delimiter-pairs` and source-ordered
`:unmatched-delimiters`. A closing delimiter pairs only with a matching opener
at the top of the structural stack, so malformed source never produces
crossing pairs. Delimiters inside strings, characters, and comments are not
delimiter tokens and therefore require no special treatment here.

</main>
</div>
