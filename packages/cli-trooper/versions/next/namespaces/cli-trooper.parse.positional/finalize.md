---
layout: reference
title: cli-trooper.parse.positional/finalize
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>
<ol>
<li><a href="consume.html">consume</a></li>
<li><a class="active" href="finalize.html">finalize</a></li>
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
<span aria-current="page">finalize</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.positional/finalize.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>/</div>
<h1><code>finalize</code></h1>
</header>

Record required primitive positionals not reached before input ended.

<hr class="signature-divider">

```roo
(finalize state command spec)
```

<hr class="signature-divider">

## Signature:

```roo
(finalize state command spec)
```

| Arg     | Description                                                   |
| ------- | ------------------------------------------------------------- |
| state   | Parser state after every supplied argument has been consumed. |
| command | The selected command literal.                                 |
| spec    | The selected canonical command specification.                 |

### Returns:

Parser state with one missing-positional diagnostic for each remaining
required positional declaration, including a required repeatable declaration
that captured no values.

</main>
</div>
