---
layout: reference
title: proofread.source/namespace-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.source.html">proofread.source</a>
<ol>
<li><a href="context.html">context</a></li>
<li><a href="diagnostics.html">diagnostics</a></li>
<li><a href="diagnostics-with-config.html">diagnostics-with-config</a></li>
<li><a class="active" href="namespace-name.html">namespace-name</a></li>
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
<span aria-current="page">namespace-name</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.source/namespace-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.source.html">proofread.source</a>/</div>
<h1><code>namespace-name</code></h1>
</header>

Return the namespace declared by parsed Roo source roots.

<hr class="signature-divider">

```roo
(namespace-name roots)
```

<hr class="signature-divider">

## Signature:

```roo
(namespace-name roots)
```

| Arg   | Description                |
| ----- | -------------------------- |
| roots | Top-level `roo.ast` nodes. |

### Returns:

The declared namespace name, or `user` when no namespace form is present.

</main>
</div>
