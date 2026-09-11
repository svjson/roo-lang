---
layout: reference
title: loom.command.bootstrap/install-node!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.bootstrap.html">loom.command.bootstrap</a>
<ol>
<li><a href="bootstrap-bang.html">bootstrap!</a></li>
<li><a href="install-dependencies-bang.html">install-dependencies!</a></li>
<li><a class="active" href="install-node-bang.html">install-node!</a></li>
<li><a href="reject-bang.html">reject!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.command.bootstrap.html">loom.command.bootstrap</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">install-node!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.bootstrap/install-node-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.bootstrap.html">loom.command.bootstrap</a>/</div>
<h1><code>install-node!</code></h1>
</header>

Install NODE and its dependency subtree into REPO-ROOT.

<hr class="signature-divider">

```roo
(install-node! repo-root installed node)
```

<hr class="signature-divider">

## Signature:

```roo
(install-node! repo-root installed node)
```

| Arg       | Description                                          |
| --------- | ---------------------------------------------------- |
| repo-root | Destination package repository.                      |
| installed | Mutable collection of package IDs already installed. |
| node      | Dependency-graph node to install.                    |

### Returns:

The package ID appended to INSTALLED, or nil when it was already installed.

</main>
</div>
