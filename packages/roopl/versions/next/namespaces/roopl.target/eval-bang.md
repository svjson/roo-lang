---
layout: reference
title: roopl.target/eval!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.target.html">roopl.target</a>
<ol>
<li><a href="close-bang.html">close!</a></li>
<li><a class="active" href="eval-bang.html">eval!</a></li>
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
<span aria-current="page">eval!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.target/eval-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.target.html">roopl.target</a>/</div>
<h1><code>eval!</code></h1>
</header>

Evaluate one request through TARGET.

<hr class="signature-divider">

```roo
(eval! target request)
```

<hr class="signature-divider">

## Signature:

```roo
(eval! target request)
```

| Arg     | Description                                                                                                                                                        |
| ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| target  | Target returned by a concrete target namespace.                                                                                                                    |
| request | Map containing string `:source`, `:namespace`, and<br>`:source-name` fields. Optional `:print-width` sets the column budget for<br>formatting a successful result. |

### Returns:

A synchronous response. Success responses contain `:status :ok`, the
resulting `:namespace`, and printable `:display` text. Error responses
contain `:status :error`, the resulting `:namespace`, an error `:kind`, and
a human-readable `:message`.

</main>
</div>
