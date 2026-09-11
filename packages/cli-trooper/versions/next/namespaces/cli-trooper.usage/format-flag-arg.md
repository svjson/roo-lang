---
layout: reference
title: cli-trooper.usage/format-flag-arg
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.usage.html">cli-trooper.usage</a>
<ol>
<li><a href="command-string.html">command-string</a></li>
<li><a href="format-flag.html">format-flag</a></li>
<li><a class="active" href="format-flag-arg.html">format-flag-arg</a></li>
<li><a href="format-flag-name.html">format-flag-name</a></li>
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
<span aria-current="page">format-flag-arg</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.usage/format-flag-arg.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.usage.html">cli-trooper.usage</a>/</div>
<h1><code>format-flag-arg</code></h1>
</header>

Resolve the value label displayed for a flag specification.

<hr class="signature-divider">

```roo
(format-flag-arg flag-spec)
```

<hr class="signature-divider">

## Signature:

```roo
(format-flag-arg flag-spec)
```

| Arg       | Description                                                          |
| --------- | -------------------------------------------------------------------- |
| flag-spec | A canonical flag specification, or supported prototype<br>shorthand. |

### Returns:

The value label string, or `nil` for a boolean flag.

</main>
</div>
