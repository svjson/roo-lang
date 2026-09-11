---
layout: reference
title: loom.command.bootstrap/reject!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.bootstrap.html">loom.command.bootstrap</a>
<ol>
<li><a href="bootstrap-bang.html">bootstrap!</a></li>
<li><a href="install-dependencies-bang.html">install-dependencies!</a></li>
<li><a href="install-node-bang.html">install-node!</a></li>
<li><a class="active" href="reject-bang.html">reject!</a></li>
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
<span aria-current="page">reject!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.bootstrap/reject-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.bootstrap.html">loom.command.bootstrap</a>/</div>
<h1><code>reject!</code></h1>
</header>

Report a bootstrap rejection.

<hr class="signature-divider">

```roo
(reject! message items)
```

<hr class="signature-divider">

## Signature:

```roo
(reject! message items)
```

| Arg     | Description                                    |
| ------- | ---------------------------------------------- |
| message | Description of why the bootstrap was rejected. |
| items   | Package IDs involved in the rejection.         |

### Returns:

Nil.

</main>
</div>
