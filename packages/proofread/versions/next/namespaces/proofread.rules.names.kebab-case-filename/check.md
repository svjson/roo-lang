---
layout: reference
title: proofread.rules.names.kebab-case-filename/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.names.kebab-case-filename.html">proofread.rules.names.kebab-case-filename</a>
<ol>
<li><a class="active" href="check.html">check</a></li>
<li><a href="scope.html">scope</a></li>
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
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.names.kebab-case-filename/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.names.kebab-case-filename.html">proofread.rules.names.kebab-case-filename</a>/</div>
<h1><code>check</code></h1>
</header>

Check a source path for underscores outside the directory-entry convention.

<hr class="signature-divider">

```roo
(check file)
```

<hr class="signature-divider">

## Signature:

```roo
(check file)
```

| Arg  | Description           |
| ---- | --------------------- |
| file | Roo source file path. |

### Returns:

A `kebab-case-filename` diagnostic when the basename contains a disallowed
underscore, otherwise `nil`.

</main>
</div>
