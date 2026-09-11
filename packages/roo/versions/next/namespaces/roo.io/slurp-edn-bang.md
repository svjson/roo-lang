---
layout: reference
title: roo.io/slurp-edn!
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
<li><a class="active" href="slurp-edn-bang.html">slurp-edn!</a></li>
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
<span aria-current="page">slurp-edn!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.io/slurp-edn-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.io.html">roo.io</a>/</div>
<h1><code>slurp-edn!</code></h1>
</header>

Read one EDN form from a text file.

<hr class="signature-divider">

```roo
(slurp-edn! path)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.io/slurp-edn! "config.edn")
=> {:name "Roo" :ok true}
```

<hr class="signature-divider">

## Signature:

```roo
(slurp-edn! path)
```

| Arg  | Description                       |
| ---- | --------------------------------- |
| path | The path of the EDN file to read. |

</main>
</div>
