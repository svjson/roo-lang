---
layout: reference
title: lookup.source.native.parse.entries/package-files
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.native.parse.entries.html">lookup.source.native.parse.entries</a>
<ol>
<li><a href="append-comment-line.html">append-comment-line</a></li>
<li><a href="block-comment-end-p.html">block-comment-end?</a></li>
<li><a href="block-comment-start-p.html">block-comment-start?</a></li>
<li><a href="comment-text.html">comment-text</a></li>
<li><a href="declaration-class.html">declaration-class</a></li>
<li><a href="declaration-kind.html">declaration-kind</a></li>
<li><a href="documentation-entry.html">documentation-entry</a></li>
<li><a href="entries.html">entries</a></li>
<li><a href="entries-bang.html">entries!</a></li>
<li><a href="files-under-excluding.html">files-under-excluding</a></li>
<li><a href="finish-comment.html">finish-comment</a></li>
<li><a href="indexed-lines.html">indexed-lines</a></li>
<li><a href="location.html">location</a></li>
<li><a href="package-entries-bang.html">package-entries!</a></li>
<li><a class="active" href="package-files.html">package-files</a></li>
<li><a href="root-entries-bang.html">root-entries!</a></li>
<li><a href="root-files.html">root-files</a></li>
<li><a href="source-file-p.html">source-file?</a></li>
<li><a href="start-comment.html">start-comment</a></li>
<li><a href="step.html">step</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.source.native.parse.entries.html">lookup.source.native.parse.entries</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">package-files</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source.native.parse.entries/package-files.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.native.parse.entries.html">lookup.source.native.parse.entries</a>/</div>
<h1><code>package-files</code></h1>
</header>

Find native source and header files declared by a package.

<hr class="signature-divider">

```roo
(package-files package-dir)
```

<hr class="signature-divider">

For each `:native-libraries` entry, scans `:source` when declared,
otherwise `:path`. `:path` names where the *compiled* payload lives
(what `loom install` copies) - for a package whose native source lives
alongside its build output, that's also where the source is, so `:path`
doubles as a reasonable default. A package with its own pre-existing
layout (source not under the same directory as the compiled artifact)
declares `:source` explicitly to override it - a vector of paths, since
headers and implementation commonly live in two separate trees.

<hr class="signature-divider">

## Signature:

```roo
(package-files package-dir)
```

| Arg         | Description                            |
| ----------- | -------------------------------------- |
| package-dir | Package root containing `package.edn`. |

### Returns:

A sorted vector of native source/header file paths.

</main>
</div>
