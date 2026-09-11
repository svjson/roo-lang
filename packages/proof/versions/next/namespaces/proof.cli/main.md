---
layout: reference
title: proof.cli/main
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.cli.html">proof.cli</a>
<ol>
<li><a href="application.html">application</a></li>
<li><a class="active" href="main.html">main</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.cli.html">proof.cli</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">main</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.cli/main.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.cli.html">proof.cli</a>/</div>
<h1><code>main</code></h1>
</header>

Run Proof through its direct command-line entrypoint.

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

Process status `0` for a passing run or help, `1` for a failing or errored
test run, or `2` for invalid command-line input.

</main>
</div>
