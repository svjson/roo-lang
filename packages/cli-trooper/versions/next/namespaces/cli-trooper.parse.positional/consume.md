---
layout: reference
title: cli-trooper.parse.positional/consume
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>
<ol>
<li><a class="active" href="consume.html">consume</a></li>
<li><a href="finalize.html">finalize</a></li>
<li><a href="positional-p.html">positional?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">consume</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.positional/consume.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>/</div>
<h1><code>consume</code></h1>
</header>

Capture one raw argument for the current primitive positional declaration.

<hr class="signature-divider">

```roo
(consume state command spec arg)
```

<hr class="signature-divider">

## Signature:

```roo
(consume state command spec arg)
```

| Arg     | Description                                   |
| ------- | --------------------------------------------- |
| state   | The current argument parser state.            |
| command | The selected command literal.                 |
| spec    | The selected canonical command specification. |
| arg     | The raw argument value to capture.            |

### Returns:

Parser state with the value or conversion diagnostic recorded and the
positional cursor advanced according to repetition.

</main>
</div>
