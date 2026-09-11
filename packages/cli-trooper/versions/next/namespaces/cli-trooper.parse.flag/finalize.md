---
layout: reference
title: cli-trooper.parse.flag/finalize
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<ol>
<li><a href="consume.html">consume</a></li>
<li><a href="consume-value.html">consume-value</a></li>
<li><a class="active" href="finalize.html">finalize</a></li>
<li><a href="flag-equals.html">flag=</a></li>
<li><a href="flag-p.html">flag?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">finalize</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.flag/finalize.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>/</div>
<h1><code>finalize</code></h1>
</header>

Record unfinished and absent required flag values.

<hr class="signature-divider">

```roo
(finalize state command flags)
```

<hr class="signature-divider">

## Signature:

```roo
(finalize state command flags)
```

| Arg     | Description                                                   |
| ------- | ------------------------------------------------------------- |
| state   | Parser state after every supplied argument has been consumed. |
| command | The selected command literal.                                 |
| flags   | The applicable canonical flag declarations.                   |

### Returns:

Parser state with a missing-option-value diagnostic for an unfinished
occurrence and one missing-required-option diagnostic for each required flag
that was never supplied.

</main>
</div>
