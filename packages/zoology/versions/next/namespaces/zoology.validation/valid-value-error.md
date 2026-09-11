---
layout: reference
title: zoology.validation/valid-value-error
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../zoology.validation.html">zoology.validation</a>
<ol>
<li><a href="append-error.html">append-error</a></li>
<li><a href="array-item-errors.html">array-item-errors</a></li>
<li><a href="array-value-error.html">array-value-error</a></li>
<li><a href="max-error.html">max-error</a></li>
<li><a href="min-error.html">min-error</a></li>
<li><a href="object-field-error.html">object-field-error</a></li>
<li><a href="required-error.html">required-error</a></li>
<li><a class="active" href="valid-value-error.html">valid-value-error</a></li>
<li><a href="validate.html">validate</a></li>
<li><a href="validate-field.html">validate-field</a></li>
<li><a href="validate-field-with-context.html">validate-field-with-context</a></li>
<li><a href="validate-with-context.html">validate-with-context</a></li>
<li><a href="validation-errors.html">validation-errors</a></li>
<li><a href="validation-errors-with-context.html">validation-errors-with-context</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">zoology next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../zoology.validation.html">zoology.validation</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">valid-value-error</span>
</nav>
<div class="version-context">
<span class="version-package">zoology</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["zoology"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/zoology.validation/valid-value-error.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../zoology.validation.html">zoology.validation</a>/</div>
<h1><code>valid-value-error</code></h1>
</header>

<hr class="signature-divider">

```roo
(valid-value-error context parent-value value {:keys [valid-values] :as field-schema})
```

<hr class="signature-divider">

## Signature:

```roo
(valid-value-error context parent-value value {:keys [valid-values] :as field-schema})
```

| Arg                                     | Description |
| --------------------------------------- | ----------- |
| context                                 |             |
| parent-value                            |             |
| value                                   |             |
| {:keys [valid-values] :as field-schema} |             |

</main>
</div>
