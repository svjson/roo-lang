---
layout: reference
title: wraparoo.lex/scan
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.lex.html">wraparoo.lex</a>
<ol>
<li><a href="boundary-p.html">boundary?</a></li>
<li><a href="delimiter-specs.html">delimiter-specs</a></li>
<li><a href="number-token-p.html">number-token?</a></li>
<li><a class="active" href="scan.html">scan</a></li>
<li><a href="scan-token.html">scan-token</a></li>
<li><a href="span-end.html">span-end</a></li>
<li><a href="string-span.html">string-span</a></li>
<li><a href="token.html">token</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.lex.html">wraparoo.lex</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">scan</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.lex/scan.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.lex.html">wraparoo.lex</a>/</div>
<h1><code>scan</code></h1>
</header>

Scan complete or incomplete Roo SOURCE into lossless lexical tokens.

<hr class="signature-divider">

```roo
(scan source)
```

<hr class="signature-divider">

## Signature:

```roo
(scan source)
```

| Arg    | Description        |
| ------ | ------------------ |
| source | Roo source string. |

### Returns:

A vector of source-ordered token maps. Their `:text` values concatenate back
to SOURCE exactly. Ranges are half-open Unicode-scalar offsets. Whitespace and
comments remain tokens, and an unterminated string or escaped character is
returned with `:status :incomplete` instead of raising.

</main>
</div>
