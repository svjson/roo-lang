---
layout: reference
title: boodle.format.lookup
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.format.lookup.html">boodle.format.lookup</a>
<ol>
<li><a href="boodle.format.lookup/append-name.html">append-name</a></li>
<li><a href="boodle.format.lookup/catalog.html">catalog</a></li>
<li><a href="boodle.format.lookup/entries-in-namespace.html">entries-in-namespace</a></li>
<li><a href="boodle.format.lookup/lookup-namespace.html">lookup-namespace</a></li>
<li><a href="boodle.format.lookup/namespace-entry.html">namespace-entry</a></li>
<li><a href="boodle.format.lookup/namespace-names.html">namespace-names</a></li>
<li><a href="boodle.format.lookup/package-entry.html">package-entry</a></li>
<li><a href="boodle.format.lookup/package-id.html">package-id</a></li>
<li><a href="boodle.format.lookup/symbol-entry.html">symbol-entry</a></li>
<li><a href="boodle.format.lookup/symbol-index.html">symbol-index</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.format.lookup</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.format.lookup.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.format.lookup

| Symbol                                                                   | Summary                                                                |
| ------------------------------------------------------------------------ | ---------------------------------------------------------------------- |
| [`append-name`](boodle.format.lookup/append-name.html)                   | Append NAME to NAMES when NAME has not already been seen.              |
| [`catalog`](boodle.format.lookup/catalog.html)                           | Convert lookup indexes to a Boodle catalog.                            |
| [`entries-in-namespace`](boodle.format.lookup/entries-in-namespace.html) | Return Boodle entries from INDEX belonging to NAME.                    |
| [`lookup-namespace`](boodle.format.lookup/lookup-namespace.html)         | Return lookup namespace metadata by NAME.                              |
| [`namespace-entry`](boodle.format.lookup/namespace-entry.html)           | Convert one namespace from a lookup index to a Boodle namespace entry. |
| [`namespace-names`](boodle.format.lookup/namespace-names.html)           | Return sorted namespace names represented by INDEX.                    |
| [`package-entry`](boodle.format.lookup/package-entry.html)               | Convert one lookup index to a Boodle package entry.                    |
| [`package-id`](boodle.format.lookup/package-id.html)                     | Return the Boodle package id for a lookup symbol index.                |
| [`symbol-entry`](boodle.format.lookup/symbol-entry.html)                 | Convert a lookup symbol entry to a Boodle entry.                       |
| [`symbol-index`](boodle.format.lookup/symbol-index.html)                 | Return the symbol index portion of a lookup index artifact.            |

</main>
</div>
