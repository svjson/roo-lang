---
layout: reference
title: cli-trooper.diagnostic/lines
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>
<ol>
<li><a href="expected-text.html">expected-text</a></li>
<li><a href="line.html">line</a></li>
<li><a class="active" href="lines.html">lines</a></li>
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
<span aria-current="page">lines</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.diagnostic/lines.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.diagnostic.html">cli-trooper.diagnostic</a>/</div>
<h1><code>lines</code></h1>
</header>

Render structured cli-trooper diagnostics as text.

<hr class="signature-divider">

```roo
(lines diagnostics)
```

<hr class="signature-divider">

## Signature:

```roo
(lines diagnostics)
```

| Arg         | Description                               |
| ----------- | ----------------------------------------- |
| diagnostics | A sequence of structured diagnostic maps. |

### Returns:

A sequence containing one human-readable line per diagnostic.

</main>
</div>
