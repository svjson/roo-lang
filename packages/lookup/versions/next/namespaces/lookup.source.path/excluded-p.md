---
layout: reference
title: lookup.source.path/excluded?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.path.html">lookup.source.path</a>
<ol>
<li><a href="canonical.html">canonical</a></li>
<li><a class="active" href="excluded-p.html">excluded?</a></li>
<li><a href="package-relative.html">package-relative</a></li>
<li><a href="within-p.html">within?</a></li>
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
<span aria-current="page">excluded?</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source.path/excluded-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.path.html">lookup.source.path</a>/</div>
<h1><code>excluded?</code></h1>
</header>

Test whether PATH equals or descends from an excluded path.

<hr class="signature-divider">

```roo
(excluded? path excludes)
```

<hr class="signature-divider">

## Signature:

```roo
(excluded? path excludes)
```

| Arg      | Description                                        |
| -------- | -------------------------------------------------- |
| path     | Path to compare against the exclusions.            |
| excludes | Paths excluded individually or as directory roots. |

### Returns:

`true` when PATH is excluded; otherwise `false`.

</main>
</div>
