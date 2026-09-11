---
layout: reference
title: lookup.source.path/within?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.path.html">lookup.source.path</a>
<ol>
<li><a href="canonical.html">canonical</a></li>
<li><a href="excluded-p.html">excluded?</a></li>
<li><a href="package-relative.html">package-relative</a></li>
<li><a class="active" href="within-p.html">within?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.source.path.html">lookup.source.path</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">within?</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source.path/within-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.path.html">lookup.source.path</a>/</div>
<h1><code>within?</code></h1>
</header>

Test whether PATH equals or descends from ROOT.

<hr class="signature-divider">

```roo
(within? path root)
```

<hr class="signature-divider">

## Signature:

```roo
(within? path root)
```

| Arg  | Description                |
| ---- | -------------------------- |
| path | Path to test.              |
| root | Candidate containing path. |

### Returns:

`true` when ROOT covers PATH; otherwise `false`.

</main>
</div>
