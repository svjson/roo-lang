---
layout: reference
title: roopl.terminal/main
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.terminal.html">roopl.terminal</a>
<ol>
<li><a class="active" href="main.html">main</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.terminal.html">roopl.terminal</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">main</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.terminal/main.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.terminal.html">roopl.terminal</a>/</div>
<h1><code>main</code></h1>
</header>

Run the interactive roopl terminal frontend.

<hr class="signature-divider">

```roo
(main args)
```

<hr class="signature-divider">

## Signature:

```roo
(main args)
```

| Arg  | Description                                                        |
| ---- | ------------------------------------------------------------------ |
| args | Command-line argument vector. No arguments are currently accepted. |

### Returns:

`nil` after the frontend exits on stream EOF or an unhandled terminal
interrupt/EOF keystroke. Submitted source is evaluated by a dedicated local
Roo runtime.

</main>
</div>
