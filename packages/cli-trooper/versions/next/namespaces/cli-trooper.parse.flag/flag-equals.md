---
layout: reference
title: cli-trooper.parse.flag/flag=
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<ol>
<li><a href="consume.html">consume</a></li>
<li><a href="consume-value.html">consume-value</a></li>
<li><a href="finalize.html">finalize</a></li>
<li><a class="active" href="flag-equals.html">flag=</a></li>
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
<span aria-current="page">flag=</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.flag/flag-equals.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>/</div>
<h1><code>flag=</code></h1>
</header>

Determine whether an argument matches a canonical flag key.

<hr class="signature-divider">

```roo
(flag= arg key)
```

<hr class="signature-divider">

## Signature:

```roo
(flag= arg key)
```

| Arg | Description                                              |
| --- | -------------------------------------------------------- |
| arg | The raw argument to compare.                             |
| key | One canonical spelling or a sequence of alias spellings. |

### Returns:

A boolean indicating that the argument exactly matches a canonical spelling.

</main>
</div>
