---
layout: reference
title: proofread.inspect.expressions/simple-call-heads
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.expressions.html">proofread.inspect.expressions</a>
<ol>
<li><a href="default-options.html">default-options</a></li>
<li><a href="default-simple-call-heads.html">default-simple-call-heads</a></li>
<li><a href="literal-p.html">literal?</a></li>
<li><a href="simple-argument-p.html">simple-argument?</a></li>
<li><a href="simple-arguments-p.html">simple-arguments?</a></li>
<li><a class="active" href="simple-call-heads.html">simple-call-heads</a></li>
<li><a href="simple-call-p.html">simple-call?</a></li>
<li><a href="simple-p.html">simple?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.expressions.html">proofread.inspect.expressions</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">simple-call-heads</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.expressions/simple-call-heads.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.expressions.html">proofread.inspect.expressions</a>/</div>
<h1><code>simple-call-heads</code></h1>
</header>

Return configured simple call heads.

<hr class="signature-divider">

```roo
(simple-call-heads options)
```

<hr class="signature-divider">

## Signature:

```roo
(simple-call-heads options)
```

| Arg     | Description       |
| ------- | ----------------- |
| options | Rule options map. |

### Returns:

A vector of call head names.

</main>
</div>
