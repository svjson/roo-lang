---
layout: reference
title: cli-trooper.feature.version/lines
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.feature.version.html">cli-trooper.feature.version</a>
<ol>
<li><a class="active" href="lines.html">lines</a></li>
<li><a href="run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.feature.version.html">cli-trooper.feature.version</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lines</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.feature.version/lines.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.feature.version.html">cli-trooper.feature.version</a>/</div>
<h1><code>lines</code></h1>
</header>

Build output for the attached application version command.

<hr class="signature-divider">

```roo
(lines config)
```

<hr class="signature-divider">

## Signature:

```roo
(lines config)
```

| Arg    | Description                                  |
| ------ | -------------------------------------------- |
| config | A canonical cli-trooper configuration model. |

### Returns:

The configured version `:output` as a sequence of lines, or one generated
`<application> <version>` line. Returns an empty sequence when the version
feature did not attach a command.

</main>
</div>
