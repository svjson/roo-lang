---
layout: reference
title: loom.command.bootstrap/bootstrap!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.bootstrap.html">loom.command.bootstrap</a>
<ol>
<li><a class="active" href="bootstrap-bang.html">bootstrap!</a></li>
<li><a href="install-dependencies-bang.html">install-dependencies!</a></li>
<li><a href="install-node-bang.html">install-node!</a></li>
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
<span aria-current="page">bootstrap!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.bootstrap/bootstrap-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.bootstrap.html">loom.command.bootstrap</a>/</div>
<h1><code>bootstrap!</code></h1>
</header>

Install the source dependency closure needed to build a package.

<hr class="signature-divider">

```roo
(bootstrap! package-dir source-root repo-root)
```

<hr class="signature-divider">

## Signature:

```roo
(bootstrap! package-dir source-root repo-root)
```

| Arg         | Description                                        |
| ----------- | -------------------------------------------------- |
| package-dir | Package whose dependencies are being bootstrapped. |
| source-root | Directory containing dependency source packages.   |
| repo-root   | Destination package repository.                    |

### Returns:

The installed package IDs, or nil when the dependency graph is invalid.

</main>
</div>
