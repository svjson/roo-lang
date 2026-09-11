---
layout: reference
title: roopl.target/close!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.target.html">roopl.target</a>
<ol>
<li><a class="active" href="close-bang.html">close!</a></li>
<li><a href="eval-bang.html">eval!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.target.html">roopl.target</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">close!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.target/close-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.target.html">roopl.target</a>/</div>
<h1><code>close!</code></h1>
</header>

Release TARGET's execution resources.

<hr class="signature-divider">

```roo
(close! target)
```

<hr class="signature-divider">

## Signature:

```roo
(close! target)
```

| Arg    | Description                                     |
| ------ | ----------------------------------------------- |
| target | Target returned by a concrete target namespace. |

### Returns:

The concrete target's close result, normally `nil`.

</main>
</div>
