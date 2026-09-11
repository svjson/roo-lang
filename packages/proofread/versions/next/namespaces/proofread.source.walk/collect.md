---
layout: reference
title: proofread.source.walk/collect
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.source.walk.html">proofread.source.walk</a>
<ol>
<li><a href="append-file-if-new.html">append-file-if-new</a></li>
<li><a href="append-files.html">append-files</a></li>
<li><a class="active" href="collect.html">collect</a></li>
<li><a href="collect-pattern.html">collect-pattern</a></li>
<li><a href="default-load-roots.html">default-load-roots</a></li>
<li><a href="directory-files.html">directory-files</a></li>
<li><a href="expand-home.html">expand-home</a></li>
<li><a href="expand-input.html">expand-input</a></li>
<li><a href="expand-pattern.html">expand-pattern</a></li>
<li><a href="glob-meta-p.html">glob-meta?</a></li>
<li><a href="ignored-directory-p.html">ignored-directory?</a></li>
<li><a href="nested-package-files.html">nested-package-files</a></li>
<li><a href="package-file.html">package-file</a></li>
<li><a href="package-files.html">package-files</a></li>
<li><a href="package-load-root-files.html">package-load-root-files</a></li>
<li><a href="package-load-roots.html">package-load-roots</a></li>
<li><a href="package-root-p.html">package-root?</a></li>
<li><a href="package-roots-under.html">package-roots-under</a></li>
<li><a href="proofread-file-p.html">proofread-file?</a></li>
<li><a href="result-path.html">result-path</a></li>
<li><a href="source-files-under.html">source-files-under</a></li>
<li><a href="source-walk-descend-p.html">source-walk-descend?</a></li>
<li><a href="traversable-directory-p.html">traversable-directory?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.source.walk.html">proofread.source.walk</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">collect</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.source.walk/collect.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.source.walk.html">proofread.source.walk</a>/</div>
<h1><code>collect</code></h1>
</header>

Resolve CLI paths and patterns into sorted, distinct proofread inputs.

<hr class="signature-divider">

```roo
(collect patterns)
```

<hr class="signature-divider">

## Signature:

```roo
(collect patterns)
```

| Arg      | Description |
| -------- | ----------- |
| patterns |             |

</main>
</div>
