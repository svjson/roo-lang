---
layout: reference
title: roo.io/walk!
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
<li><a href="normalize-path.html">normalize-path</a></li>
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
<li><a class="active" href="walk-bang.html">walk!</a></li>
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
<span aria-current="page">walk!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.io/walk-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.io.html">roo.io</a>/</div>
<h1><code>walk!</code></h1>
</header>

Walk a filesystem tree with userspace keep and descent criteria.

<hr class="signature-divider">

```roo
(walk! root)
(walk! root options)
```

<hr class="signature-divider">

Returns records containing :entry and :metadata maps in deterministic,
depth-first order. Criteria receive those two maps as arguments. Metadata
initially contains :root, :relative-path, :depth, :root?, and :symlink?.
The root is visited with relative path "." and depth 0.

By default, regular files are kept, directories are descended, hidden
entries are excluded, and directory symlinks are not descended. Directory
children are listed only after :descend? accepts their parent, and each
listing is completed before traversal continues into a child.

### Usage:

```roo
(roo.io/walk! "src")

(roo.io/walk! "."
{:descend? (fn [entry metadata]
(not (= "build" (:relative-path metadata))))
:keep? (fn [entry metadata]
(= ".roo" (roo.io/extension (:path entry))))})
```

<hr class="signature-divider">

## Signature:

```roo
(walk! root)
```

| Arg  | Description                                           |
| ---- | ----------------------------------------------------- |
| root | Existing file or directory at which traversal starts. |

<hr class="signature-divider">

## Signature:

```roo
(walk! root options)
```

| Arg     | Description                                               |
| ------- | --------------------------------------------------------- |
| root    | Existing file or directory at which traversal starts.     |
| options | Map with optional :descend?, :keep?, and :hidden? values. |

</main>
</div>
