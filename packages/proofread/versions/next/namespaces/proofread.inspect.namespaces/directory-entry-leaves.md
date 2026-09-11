---
layout: reference
title: proofread.inspect.namespaces/directory-entry-leaves
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>
<ol>
<li><a class="active" href="directory-entry-leaves.html">directory-entry-leaves</a></li>
<li><a href="directory-entry-source-p.html">directory-entry-source?</a></li>
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
<span aria-current="page">directory-entry-leaves</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.namespaces/directory-entry-leaves.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.namespaces.html">proofread.inspect.namespaces</a>/</div>
<h1><code>directory-entry-leaves</code></h1>
</header>

Return the namespace leaves valid for a directory entry source file.

<hr class="signature-divider">

```roo
(directory-entry-leaves file)
```

<hr class="signature-divider">

Namespace-root mappings replace the physical directory name at the mapped
root. Below that root, the physical directory hierarchy resumes. A load root
without a namespace-root mapping represents no namespace of its own.

<hr class="signature-divider">

## Signature:

```roo
(directory-entry-leaves file)
```

| Arg  | Description           |
| ---- | --------------------- |
| file | Roo source file path. |

### Returns:

A sequence of namespace leaf strings valid for an underscore-prefixed source
file in the containing directory.

</main>
</div>
