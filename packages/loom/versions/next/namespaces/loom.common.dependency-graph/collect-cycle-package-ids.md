---
layout: reference
title: loom.common.dependency-graph/collect-cycle-package-ids
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.common.dependency-graph.html">loom.common.dependency-graph</a>
<ol>
<li><a href="absolute-path-p.html">absolute-path?</a></li>
<li><a class="active" href="collect-cycle-package-ids.html">collect-cycle-package-ids</a></li>
<li><a href="collect-missing-package-ids.html">collect-missing-package-ids</a></li>
<li><a href="complete-p.html">complete?</a></li>
<li><a href="dependency-node-bang.html">dependency-node!</a></li>
<li><a href="dependency-nodes-bang.html">dependency-nodes!</a></li>
<li><a href="dependency-path-root.html">dependency-path-root</a></li>
<li><a href="dependency-repository-root.html">dependency-repository-root</a></li>
<li><a href="dependency-spec.html">dependency-spec</a></li>
<li><a href="dependency-spec-from-atom.html">dependency-spec-from-atom</a></li>
<li><a href="dependency-spec-from-map.html">dependency-spec-from-map</a></li>
<li><a href="dependency-specs.html">dependency-specs</a></li>
<li><a href="file-path.html">file-path</a></li>
<li><a href="keyword-name-p.html">keyword-name?</a></li>
<li><a href="normalize-name.html">normalize-name</a></li>
<li><a href="package-id.html">package-id</a></li>
<li><a href="package-node.html">package-node</a></li>
<li><a href="path-separator-p.html">path-separator?</a></li>
<li><a href="root-node-bang.html">root-node!</a></li>
<li><a href="source-dependency-node-bang.html">source-dependency-node!</a></li>
<li><a href="source-dependency-nodes-bang.html">source-dependency-nodes!</a></li>
<li><a href="source-dependency-root.html">source-dependency-root</a></li>
<li><a href="source-root-node-bang.html">source-root-node!</a></li>
<li><a href="spec-root.html">spec-root</a></li>
<li><a href="windows-drive-path-p.html">windows-drive-path?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.common.dependency-graph.html">loom.common.dependency-graph</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">collect-cycle-package-ids</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.common.dependency-graph/collect-cycle-package-ids.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.common.dependency-graph.html">loom.common.dependency-graph</a>/</div>
<h1><code>collect-cycle-package-ids</code></h1>
</header>

Collect package IDs marked as cycles beneath NODE.

<hr class="signature-divider">

```roo
(collect-cycle-package-ids node)
```

<hr class="signature-divider">

## Signature:

```roo
(collect-cycle-package-ids node)
```

| Arg  | Description                       |
| ---- | --------------------------------- |
| node | Dependency-graph node to inspect. |

### Returns:

Package IDs for every detected dependency cycle.

</main>
</div>
