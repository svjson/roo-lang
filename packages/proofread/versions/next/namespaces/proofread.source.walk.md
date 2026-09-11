---
layout: reference
title: proofread.source.walk
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.source.walk.html">proofread.source.walk</a>
<ol>
<li><a href="proofread.source.walk/append-file-if-new.html">append-file-if-new</a></li>
<li><a href="proofread.source.walk/append-files.html">append-files</a></li>
<li><a href="proofread.source.walk/collect.html">collect</a></li>
<li><a href="proofread.source.walk/collect-pattern.html">collect-pattern</a></li>
<li><a href="proofread.source.walk/default-load-roots.html">default-load-roots</a></li>
<li><a href="proofread.source.walk/directory-files.html">directory-files</a></li>
<li><a href="proofread.source.walk/expand-home.html">expand-home</a></li>
<li><a href="proofread.source.walk/expand-input.html">expand-input</a></li>
<li><a href="proofread.source.walk/expand-pattern.html">expand-pattern</a></li>
<li><a href="proofread.source.walk/glob-meta-p.html">glob-meta?</a></li>
<li><a href="proofread.source.walk/ignored-directory-p.html">ignored-directory?</a></li>
<li><a href="proofread.source.walk/nested-package-files.html">nested-package-files</a></li>
<li><a href="proofread.source.walk/package-file.html">package-file</a></li>
<li><a href="proofread.source.walk/package-files.html">package-files</a></li>
<li><a href="proofread.source.walk/package-load-root-files.html">package-load-root-files</a></li>
<li><a href="proofread.source.walk/package-load-roots.html">package-load-roots</a></li>
<li><a href="proofread.source.walk/package-root-p.html">package-root?</a></li>
<li><a href="proofread.source.walk/package-roots-under.html">package-roots-under</a></li>
<li><a href="proofread.source.walk/proofread-file-p.html">proofread-file?</a></li>
<li><a href="proofread.source.walk/result-path.html">result-path</a></li>
<li><a href="proofread.source.walk/source-files-under.html">source-files-under</a></li>
<li><a href="proofread.source.walk/source-walk-descend-p.html">source-walk-descend?</a></li>
<li><a href="proofread.source.walk/traversable-directory-p.html">traversable-directory?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.source.walk</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.source.walk.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.source.walk

| Symbol                                                                          | Summary                                                                    |
| ------------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| [`append-file-if-new`](proofread.source.walk/append-file-if-new.html)           |                                                                            |
| [`append-files`](proofread.source.walk/append-files.html)                       |                                                                            |
| [`collect`](proofread.source.walk/collect.html)                                 | Resolve CLI paths and patterns into sorted, distinct proofread inputs.     |
| [`collect-pattern`](proofread.source.walk/collect-pattern.html)                 |                                                                            |
| [`default-load-roots`](proofread.source.walk/default-load-roots.html)           |                                                                            |
| [`directory-files`](proofread.source.walk/directory-files.html)                 | Find proofread inputs below a directory using package-aware policy.        |
| [`expand-home`](proofread.source.walk/expand-home.html)                         |                                                                            |
| [`expand-input`](proofread.source.walk/expand-input.html)                       |                                                                            |
| [`expand-pattern`](proofread.source.walk/expand-pattern.html)                   | Expand one non-recursive path pattern using roo.io directory filters.      |
| [`glob-meta?`](proofread.source.walk/glob-meta-p.html)                          |                                                                            |
| [`ignored-directory?`](proofread.source.walk/ignored-directory-p.html)          | Return true when proofread should not enter a directory.                   |
| [`nested-package-files`](proofread.source.walk/nested-package-files.html)       |                                                                            |
| [`package-file`](proofread.source.walk/package-file.html)                       | Roo package manifest filename.                                             |
| [`package-files`](proofread.source.walk/package-files.html)                     | Find files selected by the package manifest rooted at PACKAGE-DIR.         |
| [`package-load-root-files`](proofread.source.walk/package-load-root-files.html) |                                                                            |
| [`package-load-roots`](proofread.source.walk/package-load-roots.html)           |                                                                            |
| [`package-root?`](proofread.source.walk/package-root-p.html)                    | Return true when a directory contains a Roo package manifest.              |
| [`package-roots-under`](proofread.source.walk/package-roots-under.html)         | Find package directories under ROOT without interpreting their manifests.  |
| [`proofread-file?`](proofread.source.walk/proofread-file-p.html)                | Return true when an entry is a Roo or EDN source file.                     |
| [`result-path`](proofread.source.walk/result-path.html)                         |                                                                            |
| [`source-files-under`](proofread.source.walk/source-files-under.html)           | Find source files under ROOT while treating nested packages as boundaries. |
| [`source-walk-descend?`](proofread.source.walk/source-walk-descend-p.html)      |                                                                            |
| [`traversable-directory?`](proofread.source.walk/traversable-directory-p.html)  |                                                                            |

</main>
</div>
