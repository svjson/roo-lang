---
layout: reference
title: proofread.source/diagnostics
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.source.html">proofread.source</a>
<ol>
<li><a href="context.html">context</a></li>
<li><a class="active" href="diagnostics.html">diagnostics</a></li>
<li><a href="diagnostics-with-config.html">diagnostics-with-config</a></li>
<li><a href="namespace-name.html">namespace-name</a></li>
<li><a href="package-name.html">package-name</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.source.html">proofread.source</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">diagnostics</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.source/diagnostics.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.source.html">proofread.source</a>/</div>
<h1><code>diagnostics</code></h1>
</header>

Run proofread rules using the discovered config for a file.

<hr class="signature-divider">

```roo
(diagnostics file & options)
```

<hr class="signature-divider">

## Signature:

```roo
(diagnostics file & options)
```

| Arg     | Description                                                 |
| ------- | ----------------------------------------------------------- |
| file    | Checked input file path.                                    |
| options | Optional input options passed to `diagnostics-with-config`. |

### Returns:

A vector of diagnostic maps.

</main>
</div>
