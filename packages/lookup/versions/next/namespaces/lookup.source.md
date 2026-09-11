---
layout: reference
title: lookup.source
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.source.html">lookup.source</a>
<ol>
<li><a href="lookup.source/extract-pkg-symbols.html">extract-pkg-symbols</a></li>
<li><a href="lookup.source/native-contributions.html">native-contributions</a></li>
<li><a href="lookup.source/package-contributions.html">package-contributions</a></li>
<li><a href="lookup.source/package-forms.html">package-forms</a></li>
<li><a href="lookup.source/package-info.html">package-info</a></li>
<li><a href="lookup.source/package-inputs.html">package-inputs</a></li>
<li><a href="lookup.source/package-load-roots.html">package-load-roots</a></li>
<li><a href="lookup.source/package-source-files.html">package-source-files</a></li>
<li><a href="lookup.source/read-manifest-bang.html">read-manifest!</a></li>
<li><a href="lookup.source/roo-contribution.html">roo-contribution</a></li>
<li><a href="lookup.source/root-symbols.html">root-symbols</a></li>
<li><a href="lookup.source/source-entry.html">source-entry</a></li>
<li><a href="lookup.source/source-entry-for-index.html">source-entry-for-index</a></li>
<li><a href="lookup.source/source-entry-p.html">source-entry?</a></li>
<li><a href="lookup.source/source-file-p.html">source-file?</a></li>
<li><a href="lookup.source/source-files-in.html">source-files-in</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.source</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.source.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.source

| Symbol                                                                | Summary                                                                     |
| --------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| [`extract-pkg-symbols`](lookup.source/extract-pkg-symbols.html)       | Extract the symbols and namespaces belonging to a package and extra roots.  |
| [`native-contributions`](lookup.source/native-contributions.html)     | Index native package sources and group results by owning source path.       |
| [`package-contributions`](lookup.source/package-contributions.html)   | Build independently replaceable Roo and native package contributions.       |
| [`package-forms`](lookup.source/package-forms.html)                   | Read the Roo forms belonging to a package.                                  |
| [`package-info`](lookup.source/package-info.html)                     | Build package metadata for lookup indexes.                                  |
| [`package-inputs`](lookup.source/package-inputs.html)                 | Discover the current Roo and native package input paths.                    |
| [`package-load-roots`](lookup.source/package-load-roots.html)         | Return the effective production source roots declared by a manifest.        |
| [`package-source-files`](lookup.source/package-source-files.html)     | Find all Roo source files declared by a package manifest.                   |
| [`read-manifest!`](lookup.source/read-manifest-bang.html)             | Read a package manifest from a package directory.                           |
| [`roo-contribution`](lookup.source/roo-contribution.html)             | Index one Roo source as a replaceable package contribution.                 |
| [`root-symbols`](lookup.source/root-symbols.html)                     | Extract symbols and namespaces from generic native source roots.            |
| [`source-entry`](lookup.source/source-entry.html)                     | Read one source file into a source-form index entry.                        |
| [`source-entry-for-index`](lookup.source/source-entry-for-index.html) | Read one source file into an index under a separately supplied source path. |
| [`source-entry?`](lookup.source/source-entry-p.html)                  |                                                                             |
| [`source-file?`](lookup.source/source-file-p.html)                    | Test whether a path names a Roo source file.                                |
| [`source-files-in`](lookup.source/source-files-in.html)               |                                                                             |

</main>
</div>
