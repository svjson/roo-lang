---
layout: reference
title: zoology.valid-values/valid-value-values
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../zoology.valid-values.html">zoology.valid-values</a>
<ol>
<li><a href="valid-value-item-value.html">valid-value-item-value</a></li>
<li><a class="active" href="valid-value-values.html">valid-value-values</a></li>
<li><a href="valid-values-items.html">valid-values-items</a></li>
<li><a href="valid-values-source-items.html">valid-values-source-items</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">zoology next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../zoology.valid-values.html">zoology.valid-values</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">valid-value-values</span>
</nav>
<div class="version-context">
<span class="version-package">zoology</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["zoology"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/zoology.valid-values/valid-value-values.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../zoology.valid-values.html">zoology.valid-values</a>/</div>
<h1><code>valid-value-values</code></h1>
</header>

<hr class="signature-divider">

```roo
(valid-value-values context {:keys [valid-values] :as field-schema} value)
```

<hr class="signature-divider">

## Signature:

```roo
(valid-value-values context {:keys [valid-values] :as field-schema} value)
```

| Arg                                     | Description |
| --------------------------------------- | ----------- |
| context                                 |             |
| {:keys [valid-values] :as field-schema} |             |
| value                                   |             |

</main>
</div>
