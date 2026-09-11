---
layout: reference
title: spool/reset!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.html">spool</a>
<ol>
<li><a href="enter-bang.html">enter!</a></li>
<li><a href="leave-bang.html">leave!</a></li>
<li><a href="record-bang.html">record!</a></li>
<li><a href="report-bang.html">report!</a></li>
<li><a class="active" href="reset-bang.html">reset!</a></li>
<li><a href="wrap.html">wrap</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../spool.html">spool</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">reset!</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool/reset-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.html">spool</a>/</div>
<h1><code>reset!</code></h1>
</header>

Discard every recorded and open context.

<hr class="signature-divider">

```roo
(reset!)
```

<hr class="signature-divider">

### Returns:

The reset spool state.

<hr class="signature-divider">

## Signature:

```roo
(reset!)
```

### Returns:

The reset spool state.

</main>
</div>
