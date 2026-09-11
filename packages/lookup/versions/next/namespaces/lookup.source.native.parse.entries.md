---
layout: reference
title: lookup.source.native.parse.entries
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.source.native.parse.entries.html">lookup.source.native.parse.entries</a>
<ol>
<li><a href="lookup.source.native.parse.entries/append-comment-line.html">append-comment-line</a></li>
<li><a href="lookup.source.native.parse.entries/block-comment-end-p.html">block-comment-end?</a></li>
<li><a href="lookup.source.native.parse.entries/block-comment-start-p.html">block-comment-start?</a></li>
<li><a href="lookup.source.native.parse.entries/comment-text.html">comment-text</a></li>
<li><a href="lookup.source.native.parse.entries/declaration-class.html">declaration-class</a></li>
<li><a href="lookup.source.native.parse.entries/declaration-kind.html">declaration-kind</a></li>
<li><a href="lookup.source.native.parse.entries/documentation-entry.html">documentation-entry</a></li>
<li><a href="lookup.source.native.parse.entries/entries.html">entries</a></li>
<li><a href="lookup.source.native.parse.entries/entries-bang.html">entries!</a></li>
<li><a href="lookup.source.native.parse.entries/files-under-excluding.html">files-under-excluding</a></li>
<li><a href="lookup.source.native.parse.entries/finish-comment.html">finish-comment</a></li>
<li><a href="lookup.source.native.parse.entries/indexed-lines.html">indexed-lines</a></li>
<li><a href="lookup.source.native.parse.entries/location.html">location</a></li>
<li><a href="lookup.source.native.parse.entries/package-entries-bang.html">package-entries!</a></li>
<li><a href="lookup.source.native.parse.entries/package-files.html">package-files</a></li>
<li><a href="lookup.source.native.parse.entries/root-entries-bang.html">root-entries!</a></li>
<li><a href="lookup.source.native.parse.entries/root-files.html">root-files</a></li>
<li><a href="lookup.source.native.parse.entries/source-file-p.html">source-file?</a></li>
<li><a href="lookup.source.native.parse.entries/start-comment.html">start-comment</a></li>
<li><a href="lookup.source.native.parse.entries/step.html">step</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.source.native.parse.entries</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.source.native.parse.entries.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.source.native.parse.entries

| Symbol                                                                                   | Summary                                                               |
| ---------------------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`append-comment-line`](lookup.source.native.parse.entries/append-comment-line.html)     |                                                                       |
| [`block-comment-end?`](lookup.source.native.parse.entries/block-comment-end-p.html)      |                                                                       |
| [`block-comment-start?`](lookup.source.native.parse.entries/block-comment-start-p.html)  |                                                                       |
| [`comment-text`](lookup.source.native.parse.entries/comment-text.html)                   |                                                                       |
| [`declaration-class`](lookup.source.native.parse.entries/declaration-class.html)         |                                                                       |
| [`declaration-kind`](lookup.source.native.parse.entries/declaration-kind.html)           |                                                                       |
| [`documentation-entry`](lookup.source.native.parse.entries/documentation-entry.html)     |                                                                       |
| [`entries`](lookup.source.native.parse.entries/entries.html)                             | Extract native documentation and identity entries from source text.   |
| [`entries!`](lookup.source.native.parse.entries/entries-bang.html)                       | Read one native source file and extract its documentation entries.    |
| [`files-under-excluding`](lookup.source.native.parse.entries/files-under-excluding.html) |                                                                       |
| [`finish-comment`](lookup.source.native.parse.entries/finish-comment.html)               |                                                                       |
| [`indexed-lines`](lookup.source.native.parse.entries/indexed-lines.html)                 |                                                                       |
| [`location`](lookup.source.native.parse.entries/location.html)                           |                                                                       |
| [`package-entries!`](lookup.source.native.parse.entries/package-entries-bang.html)       | Extract native documentation entries from a package.                  |
| [`package-files`](lookup.source.native.parse.entries/package-files.html)                 | Find native source and header files declared by a package.            |
| [`root-entries!`](lookup.source.native.parse.entries/root-entries-bang.html)             | Extract native documentation entries from generic source roots.       |
| [`root-files`](lookup.source.native.parse.entries/root-files.html)                       | Find native source and header files beneath ROOT, excluding EXCLUDES. |
| [`source-file?`](lookup.source.native.parse.entries/source-file-p.html)                  |                                                                       |
| [`start-comment`](lookup.source.native.parse.entries/start-comment.html)                 |                                                                       |
| [`step`](lookup.source.native.parse.entries/step.html)                                   |                                                                       |

</main>
</div>
