---
layout: reference
title: loom.command.list/installed-package-ids-for
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.list.html">loom.command.list</a>
<ol>
<li><a href="directory-entry-p.html">directory-entry?</a></li>
<li><a href="installed-package-ids.html">installed-package-ids</a></li>
<li><a class="active" href="installed-package-ids-for.html">installed-package-ids-for</a></li>
<li><a href="installed-version-id.html">installed-version-id</a></li>
<li><a href="list-bang.html">list!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.command.list.html">loom.command.list</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">installed-package-ids-for</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.list/installed-package-ids-for.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.list.html">loom.command.list</a>/</div>
<h1><code>installed-package-ids-for</code></h1>
</header>

<hr class="signature-divider">

```roo
(installed-package-ids-for package-entry)
```

<hr class="signature-divider">

## Signature:

```roo
(installed-package-ids-for package-entry)
```

| Arg           | Description |
| ------------- | ----------- |
| package-entry |             |

</main>
</div>
