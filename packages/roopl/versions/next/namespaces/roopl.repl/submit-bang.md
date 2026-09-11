---
layout: reference
title: roopl.repl/submit!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.repl.html">roopl.repl</a>
<ol>
<li><a href="close-bang.html">close!</a></li>
<li><a href="current-namespace.html">current-namespace</a></li>
<li><a href="make.html">make</a></li>
<li><a class="active" href="submit-bang.html">submit!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.repl.html">roopl.repl</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">submit!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.repl/submit-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.repl.html">roopl.repl</a>/</div>
<h1><code>submit!</code></h1>
</header>

Evaluate SOURCE and advance REPL submission and namespace state.

<hr class="signature-divider">

```roo
(submit! repl source & options &rest)
```

<hr class="signature-divider">

## Signature:

```roo
(submit! repl source & options &rest)
```

| Arg     | Description                                                                                |
| ------- | ------------------------------------------------------------------------------------------ |
| repl    | Mutable state returned by `make`.                                                          |
| source  | Roo source string to evaluate.                                                             |
| options | Optional map. `:print-width` sets the column budget used to<br>format the evaluated value. |
| &rest   |                                                                                            |

### Returns:

The target response. Each request receives a stable source name such as
`<repl:1>`, and the next request uses the namespace reported by this one.

</main>
</div>
