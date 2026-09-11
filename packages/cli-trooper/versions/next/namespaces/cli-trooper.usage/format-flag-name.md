---
layout: reference
title: cli-trooper.usage/format-flag-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.usage.html">cli-trooper.usage</a>
<ol>
<li><a href="command-string.html">command-string</a></li>
<li><a href="format-flag.html">format-flag</a></li>
<li><a href="format-flag-arg.html">format-flag-arg</a></li>
<li><a class="active" href="format-flag-name.html">format-flag-name</a></li>
<li><a href="format-positional.html">format-positional</a></li>
<li><a href="usage-string.html">usage-string</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.usage.html">cli-trooper.usage</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">format-flag-name</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.usage/format-flag-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.usage.html">cli-trooper.usage</a>/</div>
<h1><code>format-flag-name</code></h1>
</header>

Format one or more flag spellings for usage output.

<hr class="signature-divider">

```roo
(format-flag-name flag)
```

<hr class="signature-divider">

## Signature:

```roo
(format-flag-name flag)
```

| Arg  | Description                                              |
| ---- | -------------------------------------------------------- |
| flag | A flag spelling string or a sequence of alias spellings. |

### Returns:

A dashed flag string, or a comma-separated string for multiple aliases.

</main>
</div>
