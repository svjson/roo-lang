---
layout: reference
title: cli-trooper.config.feature.version/errors
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.feature.version.html">cli-trooper.config.feature.version</a>
<ol>
<li><a href="apply.html">apply</a></li>
<li><a class="active" href="errors.html">errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.feature.version.html">cli-trooper.config.feature.version</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">errors</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.feature.version/errors.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.feature.version.html">cli-trooper.config.feature.version</a>/</div>
<h1><code>errors</code></h1>
</header>

Validate explicit use of the default version feature.

<hr class="signature-divider">

```roo
(errors config)
```

<hr class="signature-divider">

## Signature:

```roo
(errors config)
```

| Arg    | Description                                    |
| ------ | ---------------------------------------------- |
| config | A supplied or canonical CLI configuration map. |

### Returns:

A sequence containing a missing-application-version diagnostic when the
default version feature is explicitly enabled without version metadata, or an
empty sequence. Custom map configuration is accepted without application
version metadata.

</main>
</div>
