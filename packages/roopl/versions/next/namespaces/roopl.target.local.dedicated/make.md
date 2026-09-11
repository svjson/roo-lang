---
layout: reference
title: roopl.target.local.dedicated/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.target.local.dedicated.html">roopl.target.local.dedicated</a>
<ol>
<li><a class="active" href="make.html">make</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.target.local.dedicated.html">roopl.target.local.dedicated</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.target.local.dedicated/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.target.local.dedicated.html">roopl.target.local.dedicated</a>/</div>
<h1><code>make</code></h1>
</header>

Create a target backed by a dedicated Roo runtime in this process.

<hr class="signature-divider">

```roo
(make options)
```

<hr class="signature-divider">

## Signature:

```roo
(make options)
```

| Arg     | Description                                                                                                                                                                                                                                                                                                  |
| ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| options | Configuration map. `:load-paths` may provide filesystem load<br>roots. `:package-root` may select a package whose dependencies, namespace<br>roots, native libraries, and autoloads configure the runtime. When omitted,<br>the nearest package containing the current directory is used when one<br>exists. |

### Returns:

A target accepted by `roopl.target/eval!` and `roopl.target/close!`. Its Roo
namespaces and definitions are separate from the runtime hosting roopl. This
is process-local separation, not a security sandbox.

</main>
</div>
