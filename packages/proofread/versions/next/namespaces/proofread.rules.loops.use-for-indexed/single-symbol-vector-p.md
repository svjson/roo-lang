---
layout: reference
title: proofread.rules.loops.use-for-indexed/single-symbol-vector?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.loops.use-for-indexed.html">proofread.rules.loops.use-for-indexed</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="contains-indexed-nth-p.html">contains-indexed-nth?</a></li>
<li><a href="count-condition-target.html">count-condition-target</a></li>
<li><a href="counted-while-p.html">counted-while?</a></li>
<li><a href="final-form.html">final-form</a></li>
<li><a href="increment-final-body-p.html">increment-final-body?</a></li>
<li><a href="increment-p.html">increment?</a></li>
<li><a href="indexed-nth-p.html">indexed-nth?</a></li>
<li><a href="one-node-p.html">one-node?</a></li>
<li><a class="active" href="single-symbol-vector-p.html">single-symbol-vector?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.loops.use-for-indexed.html">proofread.rules.loops.use-for-indexed</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">single-symbol-vector?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.loops.use-for-indexed/single-symbol-vector-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.loops.use-for-indexed.html">proofread.rules.loops.use-for-indexed</a>/</div>
<h1><code>single-symbol-vector?</code></h1>
</header>

Test whether a vector contains one expected symbol.

<hr class="signature-divider">

```roo
(single-symbol-vector? node symbol)
```

<hr class="signature-divider">

## Signature:

```roo
(single-symbol-vector? node symbol)
```

| Arg    | Description |
| ------ | ----------- |
| node   |             |
| symbol |             |

</main>
</div>
