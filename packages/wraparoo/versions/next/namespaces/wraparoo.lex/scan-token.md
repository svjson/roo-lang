---
layout: reference
title: wraparoo.lex/scan-token
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.lex.html">wraparoo.lex</a>
<ol>
<li><a href="boundary-p.html">boundary?</a></li>
<li><a href="delimiter-specs.html">delimiter-specs</a></li>
<li><a href="number-token-p.html">number-token?</a></li>
<li><a href="scan.html">scan</a></li>
<li><a class="active" href="scan-token.html">scan-token</a></li>
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
<span aria-current="page">scan-token</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.lex/scan-token.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.lex.html">wraparoo.lex</a>/</div>
<h1><code>scan-token</code></h1>
</header>

Scan the source token beginning at START.

<hr class="signature-divider">

```roo
(scan-token scalars start)
```

<hr class="signature-divider">

## Signature:

```roo
(scan-token scalars start)
```

| Arg     | Description                                        |
| ------- | -------------------------------------------------- |
| scalars | Complete source split into Unicode scalar strings. |
| start   | Offset of the first scalar in the token.           |

### Returns:

One lossless token map whose range begins at START and ends after at least one
scalar.

</main>
</div>
