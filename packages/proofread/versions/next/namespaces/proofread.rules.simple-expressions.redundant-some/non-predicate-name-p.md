---
layout: reference
title: proofread.rules.simple-expressions.redundant-some/non-predicate-name?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.redundant-some.html">proofread.rules.simple-expressions.redundant-some</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="non-predicate-name-p.html">non-predicate-name?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.redundant-some.html">proofread.rules.simple-expressions.redundant-some</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">non-predicate-name?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-some/non-predicate-name-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.redundant-some.html">proofread.rules.simple-expressions.redundant-some</a>/</div>
<h1><code>non-predicate-name?</code></h1>
</header>

Test whether a name is not predicate-shaped.

<hr class="signature-divider">

```roo
(non-predicate-name? name)
```

<hr class="signature-divider">

## Signature:

```roo
(non-predicate-name? name)
```

| Arg  | Description                    |
| ---- | ------------------------------ |
| name | Symbol or keyword name string. |

### Returns:

`true` when `name` is not empty, not predicate-suffixed, and not a boolean or
nil literal name.

</main>
</div>
