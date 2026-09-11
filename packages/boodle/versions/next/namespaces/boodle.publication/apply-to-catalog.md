---
layout: reference
title: boodle.publication/apply-to-catalog
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.publication.html">boodle.publication</a>
<ol>
<li><a class="active" href="apply-to-catalog.html">apply-to-catalog</a></li>
<li><a href="automatic.html">automatic</a></li>
<li><a href="build-identifier-p.html">build-identifier?</a></li>
<li><a href="classify.html">classify</a></li>
<li><a href="compare-prerelease.html">compare-prerelease</a></li>
<li><a href="compare-versions.html">compare-versions</a></li>
<li><a href="identifier-character-p.html">identifier-character?</a></li>
<li><a href="identifier-p.html">identifier?</a></li>
<li><a href="make.html">make</a></li>
<li><a href="numeric-identifier-p.html">numeric-identifier?</a></li>
<li><a href="obsolete-identities.html">obsolete-identities</a></li>
<li><a href="ordered.html">ordered</a></li>
<li><a href="parse-version.html">parse-version</a></li>
<li><a href="retention-errors.html">retention-errors</a></li>
<li><a href="validation-errors.html">validation-errors</a></li>
<li><a href="version-core.html">version-core</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.publication.html">boodle.publication</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">apply-to-catalog</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.publication/apply-to-catalog.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.publication.html">boodle.publication</a>/</div>
<h1><code>apply-to-catalog</code></h1>
</header>

Apply a public documentation identity to every package in CATALOG.

<hr class="signature-divider">

```roo
(apply-to-catalog catalog publication)
```

<hr class="signature-divider">

## Signature:

```roo
(apply-to-catalog catalog publication)
```

| Arg         | Description                                   |
| ----------- | --------------------------------------------- |
| catalog     | Imported Boodle catalog.                      |
| publication | Validated publication map returned by `make`. |

### Returns:

Catalog carrying publication metadata while package versions remain unchanged
as source metadata.

</main>
</div>
