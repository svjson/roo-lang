---
layout: reference
title: cli-trooper.diagnostic/line
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<ol>
<li><a href="expected-text.html">expected-text</a></li>
<li><a class="active" href="line.html">line</a></li>
<li><a href="lines.html">lines</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">line</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.diagnostic/line.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>/</div>
<h1><code>line</code></h1>
</header>

Render one structured cli-trooper diagnostic as text.

<hr class="signature-divider">

```roo
(line diagnostic)
```

<hr class="signature-divider">

A diagnostic carrying an explicit `:message` uses that text unchanged.
Unknown application-defined kinds are rendered from their keyword name and
the common `:argument`, `:value`, and `:expected` fields.

<hr class="signature-divider">

## Signature:

```roo
(line diagnostic)
```

| Arg        | Description                                                                                   |
| ---------- | --------------------------------------------------------------------------------------------- |
| diagnostic | A structured configuration, parsing, conversion, validation,<br>or invocation diagnostic map. |

### Returns:

One human-readable diagnostic line.

</main>
</div>
