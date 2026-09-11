---
layout: reference
title: roo.io/normalize-path
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.io.html">roo.io</a>
<ol>
<li><a href="absolute-path-bang.html">absolute-path!</a></li>
<li><a href="basename.html">basename</a></li>
<li><a href="copy-file-bang.html">copy-file!</a></li>
<li><a href="current-directory-bang.html">current-directory!</a></li>
<li><a href="directory-p.html">directory?</a></li>
<li><a href="dirname.html">dirname</a></li>
<li><a href="exists-p.html">exists?</a></li>
<li><a href="extension.html">extension</a></li>
<li><a href="file-p.html">file?</a></li>
<li><a href="home-directory-bang.html">home-directory!</a></li>
<li><a href="join-path.html">join-path</a></li>
<li><a href="list-directory-bang.html">list-directory!</a></li>
<li><a class="active" href="normalize-path.html">normalize-path</a></li>
<li><a href="parent-path.html">parent-path</a></li>
<li><a href="read-link-bang.html">read-link!</a></li>
<li><a href="remove-tree-bang.html">remove-tree!</a></li>
<li><a href="slurp-bang.html">slurp!</a></li>
<li><a href="slurp-edn-bang.html">slurp-edn!</a></li>
<li><a href="spit-bang.html">spit!</a></li>
<li><a href="spit-edn-bang.html">spit-edn!</a></li>
<li><a href="stat-bang.html">stat!</a></li>
<li><a href="symlink-bang.html">symlink!</a></li>
<li><a href="symlink-p.html">symlink?</a></li>
<li><a href="walk-bang.html">walk!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.io.html">roo.io</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">normalize-path</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.io/normalize-path.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.io.html">roo.io</a>/</div>
<h1><code>normalize-path</code></h1>
</header>

Normalize a path lexically without accessing the filesystem.

<hr class="signature-divider">

```roo
(normalize-path path)
```

<hr class="signature-divider">

Relative paths remain relative and absolute paths remain absolute. The
returned path uses forward slashes on every platform. A `nil` path remains
`nil`.

### Usage:

```roo
(roo.io/normalize-path "src/./app/../main") => "src/main"
(roo.io/normalize-path nil) => nil
```

<hr class="signature-divider">

## Signature:

```roo
(normalize-path path)
```

| Arg  | Description                             |
| ---- | --------------------------------------- |
| path | The path string to normalize, or `nil`. |

</main>
</div>
