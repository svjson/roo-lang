---
layout: reference
title: lookup.index.incremental
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.index.incremental.html">lookup.index.incremental</a>
<ol>
<li><a href="lookup.index.incremental/index-metadata.html">INDEX-METADATA</a></li>
<li><a href="lookup.index.incremental/compatible-state-p.html">compatible-state?</a></li>
<li><a href="lookup.index.incremental/full-build-bang.html">full-build!</a></li>
<li><a href="lookup.index.incremental/index-with-state.html">index-with-state</a></li>
<li><a href="lookup.index.incremental/manifest-path.html">manifest-path</a></li>
<li><a href="lookup.index.incremental/manifest-snapshot.html">manifest-snapshot</a></li>
<li><a href="lookup.index.incremental/modified-ms.html">modified-ms</a></li>
<li><a href="lookup.index.incremental/native-path-p.html">native-path?</a></li>
<li><a href="lookup.index.incremental/package-manifest-current-p.html">package-manifest-current?</a></li>
<li><a href="lookup.index.incremental/persist-contributions-bang.html">persist-contributions!</a></li>
<li><a href="lookup.index.incremental/published-contribution.html">published-contribution</a></li>
<li><a href="lookup.index.incremental/reconcile-bang.html">reconcile!</a></li>
<li><a href="lookup.index.incremental/shard-relative-path.html">shard-relative-path</a></li>
<li><a href="lookup.index.incremental/snapshot.html">snapshot</a></li>
<li><a href="lookup.index.incremental/source-kind.html">source-kind</a></li>
<li><a href="lookup.index.incremental/state-options.html">state-options</a></li>
<li><a href="lookup.index.incremental/unique-paths.html">unique-paths</a></li>
<li><a href="lookup.index.incremental/update-roo-sources-bang.html">update-roo-sources!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.index.incremental</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.index.incremental.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.index.incremental

| Symbol                                                                                  | Summary                                                                       |
| --------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------- |
| [`INDEX-METADATA`](lookup.index.incremental/index-metadata.html)                        | Format identities and schema version for persisted incremental index data.    |
| [`compatible-state?`](lookup.index.incremental/compatible-state-p.html)                 | Test whether MANIFEST can be reused for OPTIONS and METADATA.                 |
| [`full-build!`](lookup.index.incremental/full-build-bang.html)                          | Rebuild, persist, and publish complete incremental index state.               |
| [`index-with-state`](lookup.index.incremental/index-with-state.html)                    | Update or initialize a dynamic lookup index state store.                      |
| [`manifest-path`](lookup.index.incremental/manifest-path.html)                          | Return the manifest path beneath STATE-DIR.                                   |
| [`manifest-snapshot`](lookup.index.incremental/manifest-snapshot.html)                  | Assemble a complete snapshot from the shards referenced by MANIFEST.          |
| [`modified-ms`](lookup.index.incremental/modified-ms.html)                              | Return PATH's filesystem modification time in milliseconds.                   |
| [`native-path?`](lookup.index.incremental/native-path-p.html)                           | Test whether PATH is recorded or classified as a native source.               |
| [`package-manifest-current?`](lookup.index.incremental/package-manifest-current-p.html) | Test whether the package manifest metadata recorded in MANIFEST is current.   |
| [`persist-contributions!`](lookup.index.incremental/persist-contributions-bang.html)    | Persist CONTRIBUTIONS as revisioned shards and return their manifest entries. |
| [`published-contribution`](lookup.index.incremental/published-contribution.html)        | Remove incremental path bookkeeping from a source contribution.               |
| [`reconcile!`](lookup.index.incremental/reconcile-bang.html)                            | Reconcile persisted state against all current package inputs by mtime.        |
| [`shard-relative-path`](lookup.index.incremental/shard-relative-path.html)              | Return the state-relative shard path for source ID at REVISION.               |
| [`snapshot`](lookup.index.incremental/snapshot.html)                                    | Assemble a complete symbol-index snapshot from source CONTRIBUTIONS.          |
| [`source-kind`](lookup.index.incremental/source-kind.html)                              | Classify PATH as a Roo or native index source.                                |
| [`state-options`](lookup.index.incremental/state-options.html)                          | Select and normalize the index options that identify compatible state.        |
| [`unique-paths`](lookup.index.incremental/unique-paths.html)                            | Return PATHS in encounter order with duplicate paths removed.                 |
| [`update-roo-sources!`](lookup.index.incremental/update-roo-sources-bang.html)          | Replace or remove Roo source contributions and publish one revision delta.    |

</main>
</div>
