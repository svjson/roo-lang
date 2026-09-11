---
layout: reference
title: roopl.repl/current-namespace
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.repl.html">roopl.repl</a>
<ol>
<li><a href="close-bang.html">close!</a></li>
<li><a class="active" href="current-namespace.html">current-namespace</a></li>
<li><a href="make.html">make</a></li>
<li><a href="submit-bang.html">submit!</a></li>
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
<span aria-current="page">current-namespace</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.repl/current-namespace.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.repl.html">roopl.repl</a>/</div>
<h1><code>current-namespace</code></h1>
</header>

Return the namespace used for the next submission.

<hr class="signature-divider">

```roo
(current-namespace repl)
```

<hr class="signature-divider">

## Signature:

```roo
(current-namespace repl)
```

| Arg  | Description                       |
| ---- | --------------------------------- |
| repl | Mutable state returned by `make`. |

### Returns:

The current namespace name as a string. New REPL state starts in `user`.

</main>
</div>
