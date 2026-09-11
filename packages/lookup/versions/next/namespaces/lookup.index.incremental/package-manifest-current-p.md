---
layout: reference
title: lookup.index.incremental/package-manifest-current?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.index.incremental.html">lookup.index.incremental</a>
<ol>
<li><a href="index-metadata.html">INDEX-METADATA</a></li>
<li><a href="compatible-state-p.html">compatible-state?</a></li>
<li><a href="full-build-bang.html">full-build!</a></li>
<li><a href="index-with-state.html">index-with-state</a></li>
<li><a href="manifest-path.html">manifest-path</a></li>
<li><a href="manifest-snapshot.html">manifest-snapshot</a></li>
<li><a href="modified-ms.html">modified-ms</a></li>
<li><a href="native-path-p.html">native-path?</a></li>
<li><a class="active" href="package-manifest-current-p.html">package-manifest-current?</a></li>
<li><a href="persist-contributions-bang.html">persist-contributions!</a></li>
<li><a href="published-contribution.html">published-contribution</a></li>
<li><a href="reconcile-bang.html">reconcile!</a></li>
<li><a href="shard-relative-path.html">shard-relative-path</a></li>
<li><a href="snapshot.html">snapshot</a></li>
<li><a href="source-kind.html">source-kind</a></li>
<li><a href="state-options.html">state-options</a></li>
<li><a href="unique-paths.html">unique-paths</a></li>
<li><a href="update-roo-sources-bang.html">update-roo-sources!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.index.incremental.html">lookup.index.incremental</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">package-manifest-current?</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.index.incremental/package-manifest-current-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.index.incremental.html">lookup.index.incremental</a>/</div>
<h1><code>package-manifest-current?</code></h1>
</header>

Test whether the package manifest metadata recorded in MANIFEST is current.

<hr class="signature-divider">

```roo
(package-manifest-current? {:keys [package-manifest]})
```

<hr class="signature-divider">

## Signature:

```roo
(package-manifest-current? {:keys [package-manifest]})
```

| Arg                        | Description |
| -------------------------- | ----------- |
| {:keys [package-manifest]} |             |

</main>
</div>
