---
layout: reference
title: cli-trooper.feature.version/run!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.feature.version.html">cli-trooper.feature.version</a>
<ol>
<li><a href="lines.html">lines</a></li>
<li><a class="active" href="run-bang.html">run!</a></li>
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
<span aria-current="page">run!</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.feature.version/run-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.feature.version.html">cli-trooper.feature.version</a>/</div>
<h1><code>run!</code></h1>
</header>

Print output for a command-shaped version invocation.

<hr class="signature-divider">

```roo
(run! context options)
```

<hr class="signature-divider">

## Signature:

```roo
(run! context options)
```

| Arg     | Description                                                                                                                                                                |
| ------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| context | The invocation context augmented with the canonical model under<br>`:cli-trooper/config`. An optional `:cli-trooper/print!` function replaces<br>the default line printer. |
| options | The fully resolved invocation options; version output does not<br>use them directly.                                                                                       |

### Returns:

The version lines after printing them.

</main>
</div>
