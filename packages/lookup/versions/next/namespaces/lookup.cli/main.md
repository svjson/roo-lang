---
layout: reference
title: lookup.cli/main
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.cli.html">lookup.cli</a>
<ol>
<li><a href="application.html">application</a></li>
<li><a class="active" href="main.html">main</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.cli.html">lookup.cli</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">main</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.cli/main.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.cli.html">lookup.cli</a>/</div>
<h1><code>main</code></h1>
</header>

Run the Lookup command-line application.

<hr class="signature-divider">

```roo
(main args)
```

<hr class="signature-divider">

## Signature:

```roo
(main args)
```

| Arg  | Description                   |
| ---- | ----------------------------- |
| args | Command-line argument vector. |

### Returns:

A process-compatible exit code.

</main>
</div>
