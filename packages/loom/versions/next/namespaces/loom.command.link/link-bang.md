---
layout: reference
title: loom.command.link/link!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.link.html">loom.command.link</a>
<ol>
<li><a class="active" href="link-bang.html">link!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.command.link.html">loom.command.link</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">link!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.link/link-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.link.html">loom.command.link</a>/</div>
<h1><code>link!</code></h1>
</header>

<hr class="signature-divider">

```roo
(link! package-dir repo-root)
```

<hr class="signature-divider">

## Signature:

```roo
(link! package-dir repo-root)
```

| Arg         | Description |
| ----------- | ----------- |
| package-dir |             |
| repo-root   |             |

</main>
</div>
