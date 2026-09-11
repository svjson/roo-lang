---
layout: reference
title: loom.common.dependency-graph
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="loom.common.dependency-graph.html">loom.common.dependency-graph</a>
<ol>
<li><a href="loom.common.dependency-graph/absolute-path-p.html">absolute-path?</a></li>
<li><a href="loom.common.dependency-graph/collect-cycle-package-ids.html">collect-cycle-package-ids</a></li>
<li><a href="loom.common.dependency-graph/collect-missing-package-ids.html">collect-missing-package-ids</a></li>
<li><a href="loom.common.dependency-graph/complete-p.html">complete?</a></li>
<li><a href="loom.common.dependency-graph/dependency-node-bang.html">dependency-node!</a></li>
<li><a href="loom.common.dependency-graph/dependency-nodes-bang.html">dependency-nodes!</a></li>
<li><a href="loom.common.dependency-graph/dependency-path-root.html">dependency-path-root</a></li>
<li><a href="loom.common.dependency-graph/dependency-repository-root.html">dependency-repository-root</a></li>
<li><a href="loom.common.dependency-graph/dependency-spec.html">dependency-spec</a></li>
<li><a href="loom.common.dependency-graph/dependency-spec-from-atom.html">dependency-spec-from-atom</a></li>
<li><a href="loom.common.dependency-graph/dependency-spec-from-map.html">dependency-spec-from-map</a></li>
<li><a href="loom.common.dependency-graph/dependency-specs.html">dependency-specs</a></li>
<li><a href="loom.common.dependency-graph/file-path.html">file-path</a></li>
<li><a href="loom.common.dependency-graph/keyword-name-p.html">keyword-name?</a></li>
<li><a href="loom.common.dependency-graph/normalize-name.html">normalize-name</a></li>
<li><a href="loom.common.dependency-graph/package-id.html">package-id</a></li>
<li><a href="loom.common.dependency-graph/package-node.html">package-node</a></li>
<li><a href="loom.common.dependency-graph/path-separator-p.html">path-separator?</a></li>
<li><a href="loom.common.dependency-graph/root-node-bang.html">root-node!</a></li>
<li><a href="loom.common.dependency-graph/source-dependency-node-bang.html">source-dependency-node!</a></li>
<li><a href="loom.common.dependency-graph/source-dependency-nodes-bang.html">source-dependency-nodes!</a></li>
<li><a href="loom.common.dependency-graph/source-dependency-root.html">source-dependency-root</a></li>
<li><a href="loom.common.dependency-graph/source-root-node-bang.html">source-root-node!</a></li>
<li><a href="loom.common.dependency-graph/spec-root.html">spec-root</a></li>
<li><a href="loom.common.dependency-graph/windows-drive-path-p.html">windows-drive-path?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">loom.common.dependency-graph</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/loom.common.dependency-graph.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# loom.common.dependency-graph

| Symbol                                                                                         | Summary                                                         |
| ---------------------------------------------------------------------------------------------- | --------------------------------------------------------------- |
| [`absolute-path?`](loom.common.dependency-graph/absolute-path-p.html)                          |                                                                 |
| [`collect-cycle-package-ids`](loom.common.dependency-graph/collect-cycle-package-ids.html)     | Collect package IDs marked as cycles beneath NODE.              |
| [`collect-missing-package-ids`](loom.common.dependency-graph/collect-missing-package-ids.html) |                                                                 |
| [`complete?`](loom.common.dependency-graph/complete-p.html)                                    |                                                                 |
| [`dependency-node!`](loom.common.dependency-graph/dependency-node-bang.html)                   |                                                                 |
| [`dependency-nodes!`](loom.common.dependency-graph/dependency-nodes-bang.html)                 |                                                                 |
| [`dependency-path-root`](loom.common.dependency-graph/dependency-path-root.html)               |                                                                 |
| [`dependency-repository-root`](loom.common.dependency-graph/dependency-repository-root.html)   |                                                                 |
| [`dependency-spec`](loom.common.dependency-graph/dependency-spec.html)                         |                                                                 |
| [`dependency-spec-from-atom`](loom.common.dependency-graph/dependency-spec-from-atom.html)     |                                                                 |
| [`dependency-spec-from-map`](loom.common.dependency-graph/dependency-spec-from-map.html)       |                                                                 |
| [`dependency-specs`](loom.common.dependency-graph/dependency-specs.html)                       |                                                                 |
| [`file-path`](loom.common.dependency-graph/file-path.html)                                     |                                                                 |
| [`keyword-name?`](loom.common.dependency-graph/keyword-name-p.html)                            |                                                                 |
| [`normalize-name`](loom.common.dependency-graph/normalize-name.html)                           |                                                                 |
| [`package-id`](loom.common.dependency-graph/package-id.html)                                   |                                                                 |
| [`package-node`](loom.common.dependency-graph/package-node.html)                               |                                                                 |
| [`path-separator?`](loom.common.dependency-graph/path-separator-p.html)                        |                                                                 |
| [`root-node!`](loom.common.dependency-graph/root-node-bang.html)                               |                                                                 |
| [`source-dependency-node!`](loom.common.dependency-graph/source-dependency-node-bang.html)     | Build a source dependency node and its transitive dependencies. |
| [`source-dependency-nodes!`](loom.common.dependency-graph/source-dependency-nodes-bang.html)   | Build source dependency nodes for SPECS.                        |
| [`source-dependency-root`](loom.common.dependency-graph/source-dependency-root.html)           | Resolve a dependency to its source-package directory.           |
| [`source-root-node!`](loom.common.dependency-graph/source-root-node-bang.html)                 | Build a dependency graph from source packages.                  |
| [`spec-root`](loom.common.dependency-graph/spec-root.html)                                     |                                                                 |
| [`windows-drive-path?`](loom.common.dependency-graph/windows-drive-path-p.html)                |                                                                 |

</main>
</div>
