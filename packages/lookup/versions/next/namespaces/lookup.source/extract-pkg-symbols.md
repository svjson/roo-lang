---
layout: reference
title: lookup.source/extract-pkg-symbols
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.html">lookup.source</a>
<ol>
<li><a class="active" href="extract-pkg-symbols.html">extract-pkg-symbols</a></li>
<li><a href="native-contributions.html">native-contributions</a></li>
<li><a href="package-contributions.html">package-contributions</a></li>
<li><a href="package-forms.html">package-forms</a></li>
<li><a href="package-info.html">package-info</a></li>
<li><a href="package-inputs.html">package-inputs</a></li>
<li><a href="package-load-roots.html">package-load-roots</a></li>
<li><a href="package-source-files.html">package-source-files</a></li>
<li><a href="read-manifest-bang.html">read-manifest!</a></li>
<li><a href="roo-contribution.html">roo-contribution</a></li>
<li><a href="root-symbols.html">root-symbols</a></li>
<li><a href="source-entry.html">source-entry</a></li>
<li><a href="source-entry-for-index.html">source-entry-for-index</a></li>
<li><a href="source-entry-p.html">source-entry?</a></li>
<li><a href="source-file-p.html">source-file?</a></li>
<li><a href="source-files-in.html">source-files-in</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.source.html">lookup.source</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">extract-pkg-symbols</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source/extract-pkg-symbols.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.html">lookup.source</a>/</div>
<h1><code>extract-pkg-symbols</code></h1>
</header>

Extract the symbols and namespaces belonging to a package and extra roots.

<hr class="signature-divider">

```roo
(extract-pkg-symbols package-dir roots excludes)
```

<hr class="signature-divider">

## Signature:

```roo
(extract-pkg-symbols package-dir roots excludes)
```

| Arg         | Description                            |
| ----------- | -------------------------------------- |
| package-dir | Package root containing `package.edn`. |
| roots       | Additional native source roots.        |
| excludes    | Paths excluded from additional roots.  |

### Returns:

Package metadata, source paths, namespaces, and symbols.

</main>
</div>
