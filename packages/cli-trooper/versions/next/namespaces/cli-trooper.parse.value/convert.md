---
layout: reference
title: cli-trooper.parse.value/convert
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.value.html">cli-trooper.parse.value</a>
<ol>
<li><a class="active" href="convert.html">convert</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.value.html">cli-trooper.parse.value</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">convert</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.value/convert.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.value.html">cli-trooper.parse.value</a>/</div>
<h1><code>convert</code></h1>
</header>

Validate and convert one raw argument according to a canonical value spec.

<hr class="signature-divider">

```roo
(convert {:keys [delimiter parse] :as spec} raw)
```

<hr class="signature-divider">

A `:delimiter` converts every separated part independently and returns their
values as a vector. When the specification also has an application-defined
`:parse` function, that function receives the converted vector; without a
delimiter it receives the raw argument. It must return the same
`{:value ...}` or `{:error ...}` result shape as this function.

<hr class="signature-divider">

## Signature:

```roo
(convert {:keys [delimiter parse] :as spec} raw)
```

| Arg                                | Description                 |
| ---------------------------------- | --------------------------- |
| {:keys [delimiter parse] :as spec} |                             |
| raw                                | The raw CLI argument value. |

### Returns:

A map containing the converted `:value`, or an `:error` describing why the
raw value is invalid.

- `spec`: A canonical flag or positional value specification.

### Returns:

A map containing the converted `:value`, or an `:error` describing why the
raw value is invalid.

</main>
</div>
