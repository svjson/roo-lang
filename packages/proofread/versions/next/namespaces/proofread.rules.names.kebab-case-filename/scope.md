---
layout: reference
title: proofread.rules.names.kebab-case-filename/scope
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.names.kebab-case-filename.html">proofread.rules.names.kebab-case-filename</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="scope.html">scope</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.names.kebab-case-filename.html">proofread.rules.names.kebab-case-filename</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">scope</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.names.kebab-case-filename/scope.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.names.kebab-case-filename.html">proofread.rules.names.kebab-case-filename</a>/</div>
<h1><code>scope</code></h1>
</header>

Return the diagnostic scope for filename checks.

<hr class="signature-divider">

```roo
(scope)
```

<hr class="signature-divider">

### Returns:

The `:file` scope keyword.

<hr class="signature-divider">

## Signature:

```roo
(scope)
```

### Returns:

The `:file` scope keyword.

</main>
</div>
