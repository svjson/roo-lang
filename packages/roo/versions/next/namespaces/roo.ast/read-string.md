---
layout: reference
title: roo.ast/read-string
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.ast.html">roo.ast</a>
<ol>
<li><a class="active" href="read-string.html">read-string</a></li>
<li><a href="slurp-bang.html">slurp!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.ast.html">roo.ast</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">read-string</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.ast/read-string.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.ast.html">roo.ast</a>/</div>
<h1><code>read-string</code></h1>
</header>

Read a Roo source string into top-level AST node entries.

<hr class="signature-divider">

```roo
(read-string source-str)
(read-string source-str opts)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.ast/read-string "(def answer 42)")
(roo.ast/read-string "(def answer 42)" {:path "inline.roo"})
```

<hr class="signature-divider">

## Signature:

```roo
(read-string source-str)
```

| Arg        | Description              |
| ---------- | ------------------------ |
| source-str | The source text to read. |

<hr class="signature-divider">

## Signature:

```roo
(read-string source-str opts)
```

| Arg        | Description                                       |
| ---------- | ------------------------------------------------- |
| source-str | The source text to read.                          |
| opts       | Map with optional :path used in source locations. |

</main>
</div>
