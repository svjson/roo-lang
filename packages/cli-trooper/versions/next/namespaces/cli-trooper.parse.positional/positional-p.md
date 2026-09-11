---
layout: reference
title: cli-trooper.parse.positional/positional?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>
<ol>
<li><a href="consume.html">consume</a></li>
<li><a href="finalize.html">finalize</a></li>
<li><a class="active" href="positional-p.html">positional?</a></li>
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
<span aria-current="page">positional?</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.positional/positional-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>/</div>
<h1><code>positional?</code></h1>
</header>

Determine whether an argument has the prototype's positional token shape.

<hr class="signature-divider">

```roo
(positional? arg)
```

<hr class="signature-divider">

## Signature:

```roo
(positional? arg)
```

| Arg | Description                        |
| --- | ---------------------------------- |
| arg | The CLI argument value to inspect. |

### Returns:

A boolean indicating that `arg` is a string without a leading dash.

</main>
</div>
