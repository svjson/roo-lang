---
layout: reference
title: proofread.inspect.namespaces/directory-entry-source?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>
<ol>
<li><a href="directory-entry-leaves.html">directory-entry-leaves</a></li>
<li><a class="active" href="directory-entry-source-p.html">directory-entry-source?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">directory-entry-source?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.namespaces/directory-entry-source-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>/</div>
<h1><code>directory-entry-source?</code></h1>
</header>

Test whether a source path is a namespace directory entry file.

<hr class="signature-divider">

```roo
(directory-entry-source? file)
```

<hr class="signature-divider">

## Signature:

```roo
(directory-entry-source? file)
```

| Arg  | Description           |
| ---- | --------------------- |
| file | Roo source file path. |

### Returns:

`true` when the basename is the underscore-prefixed logical namespace leaf
for its directory, otherwise `false`.

</main>
</div>
