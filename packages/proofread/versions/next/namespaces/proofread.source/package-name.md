---
layout: reference
title: proofread.source/package-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.source.html">proofread.source</a>
<ol>
<li><a href="context.html">context</a></li>
<li><a href="diagnostics.html">diagnostics</a></li>
<li><a href="diagnostics-with-config.html">diagnostics-with-config</a></li>
<li><a href="namespace-name.html">namespace-name</a></li>
<li><a class="active" href="package-name.html">package-name</a></li>
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
<span aria-current="page">package-name</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.source/package-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.source.html">proofread.source</a>/</div>
<h1><code>package-name</code></h1>
</header>

Return the package name for a source file.

<hr class="signature-divider">

```roo
(package-name file)
```

<hr class="signature-divider">

## Signature:

```roo
(package-name file)
```

| Arg  | Description       |
| ---- | ----------------- |
| file | Source file path. |

### Returns:

The nearest enclosing package name, or `nil` when the file is outside a Roo
package.

</main>
</div>
