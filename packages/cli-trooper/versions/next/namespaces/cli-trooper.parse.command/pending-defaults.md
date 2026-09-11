---
layout: reference
title: cli-trooper.parse.command/pending-defaults
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<ol>
<li><a href="command-flag.html">command-flag</a></li>
<li><a class="active" href="pending-defaults.html">pending-defaults</a></li>
<li><a href="select.html">select</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">pending-defaults</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.command/pending-defaults.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>/</div>
<h1><code>pending-defaults</code></h1>
</header>

Collect one command's unresolved canonical defaults.

<hr class="signature-divider">

```roo
(pending-defaults spec)
```

<hr class="signature-divider">

## Signature:

```roo
(pending-defaults spec)
```

| Arg  | Description                                                                      |
| ---- | -------------------------------------------------------------------------------- |
| spec | A canonical command specification containing flags and primitive<br>positionals. |

### Returns:

A flat map from every declared destination to its unresolved literal default
or default resolver function.

</main>
</div>
