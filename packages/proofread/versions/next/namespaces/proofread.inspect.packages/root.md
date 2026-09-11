---
layout: reference
title: proofread.inspect.packages/root
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.packages.html">proofread.inspect.packages</a>
<ol>
<li><a class="active" href="root.html">root</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.packages.html">proofread.inspect.packages</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">root</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.packages/root.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.packages.html">proofread.inspect.packages</a>/</div>
<h1><code>root</code></h1>
</header>

Find the nearest package root at or above a directory.

<hr class="signature-divider">

```roo
(root dir)
```

<hr class="signature-divider">

## Signature:

```roo
(root dir)
```

| Arg | Description                          |
| --- | ------------------------------------ |
| dir | Absolute or relative directory path. |

### Returns:

The nearest directory containing `package.edn`, or `nil` when none exists.

</main>
</div>
