---
layout: reference
title: loom.command.init/init!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.init.html">loom.command.init</a>
<ol>
<li><a class="active" href="init-bang.html">init!</a></li>
<li><a href="manifest-source.html">manifest-source</a></li>
<li><a href="package-name.html">package-name</a></li>
<li><a href="package-name-from-dir.html">package-name-from-dir</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.command.init.html">loom.command.init</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">init!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.init/init-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.init.html">loom.command.init</a>/</div>
<h1><code>init!</code></h1>
</header>

<hr class="signature-divider">

```roo
(init! package-dir name version)
```

<hr class="signature-divider">

## Signature:

```roo
(init! package-dir name version)
```

| Arg         | Description |
| ----------- | ----------- |
| package-dir |             |
| name        |             |
| version     |             |

</main>
</div>
