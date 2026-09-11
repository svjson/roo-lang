---
layout: reference
title: wraparoo.lex/string-span
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.lex.html">wraparoo.lex</a>
<ol>
<li><a href="boundary-p.html">boundary?</a></li>
<li><a href="delimiter-specs.html">delimiter-specs</a></li>
<li><a href="number-token-p.html">number-token?</a></li>
<li><a href="scan.html">scan</a></li>
<li><a href="scan-token.html">scan-token</a></li>
<li><a href="span-end.html">span-end</a></li>
<li><a class="active" href="string-span.html">string-span</a></li>
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
<span aria-current="page">string-span</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.lex/string-span.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.lex.html">wraparoo.lex</a>/</div>
<h1><code>string-span</code></h1>
</header>

Analyze a string token beginning at START without requiring its closing quote.

<hr class="signature-divider">

```roo
(string-span scalars start)
```

<hr class="signature-divider">

## Signature:

```roo
(string-span scalars start)
```

| Arg     | Description                                        |
| ------- | -------------------------------------------------- |
| scalars | Complete source split into Unicode scalar strings. |
| start   | Offset of the opening double quote.                |

### Returns:

A map containing exclusive `:end` and `:status :complete|:incomplete`. Escaped
quotes remain inside the string.

</main>
</div>
