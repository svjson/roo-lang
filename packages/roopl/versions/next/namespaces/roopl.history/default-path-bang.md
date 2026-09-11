---
layout: reference
title: roopl.history/default-path!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roopl.history.html">roopl.history</a>
<ol>
<li><a class="active" href="default-path-bang.html">default-path!</a></li>
<li><a href="load-bang.html">load!</a></li>
<li><a href="max-entries.html">max-entries</a></li>
<li><a href="record-bang.html">record!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roopl.history.html">roopl.history</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">default-path!</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roopl.history/default-path-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roopl.history.html">roopl.history</a>/</div>
<h1><code>default-path!</code></h1>
</header>

Return the default per-user roopl history path.

<hr class="signature-divider">

```roo
(default-path!)
```

<hr class="signature-divider">

## Signature:

```roo
(default-path!)
```

### Returns:

The `.roopl_history` path under the active filesystem's home directory.

None.

### Returns:

The `.roopl_history` path under the active filesystem's home directory.

</main>
</div>
