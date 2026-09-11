---
layout: reference
title: cli-trooper.config.validate/input-flag-errors
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>
<ol>
<li><a href="flag-errors.html">flag-errors</a></li>
<li><a href="input-errors.html">input-errors</a></li>
<li><a class="active" href="input-flag-errors.html">input-flag-errors</a></li>
<li><a href="missing-mixin-errors.html">missing-mixin-errors</a></li>
<li><a href="model-errors.html">model-errors</a></li>
<li><a href="validate-flag-entry.html">validate-flag-entry</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">input-flag-errors</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.validate/input-flag-errors.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.validate.html">cli-trooper.config.validate</a>/</div>
<h1><code>input-flag-errors</code></h1>
</header>

Find flag collisions that would be hidden by canonical map association.

<hr class="signature-divider">

```roo
(input-flag-errors command flags)
```

<hr class="signature-divider">

## Signature:

```roo
(input-flag-errors command flags)
```

| Arg     | Description                                          |
| ------- | ---------------------------------------------------- |
| command | The owning command literal, or `nil` for root flags. |
| flags   | A supplied flag declaration map.                     |

### Returns:

A vector of structured `:duplicate-flag-name` diagnostics.

</main>
</div>
