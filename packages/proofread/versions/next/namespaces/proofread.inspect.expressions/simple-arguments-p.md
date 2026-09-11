---
layout: reference
title: proofread.inspect.expressions/simple-arguments?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.expressions.html">proofread.inspect.expressions</a>
<ol>
<li><a href="default-options.html">default-options</a></li>
<li><a href="default-simple-call-heads.html">default-simple-call-heads</a></li>
<li><a href="literal-p.html">literal?</a></li>
<li><a href="simple-argument-p.html">simple-argument?</a></li>
<li><a class="active" href="simple-arguments-p.html">simple-arguments?</a></li>
<li><a href="simple-call-heads.html">simple-call-heads</a></li>
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
<span aria-current="page">simple-arguments?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.expressions/simple-arguments-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.expressions.html">proofread.inspect.expressions</a>/</div>
<h1><code>simple-arguments?</code></h1>
</header>

Test whether call arguments from `index` are simple.

<hr class="signature-divider">

```roo
(simple-arguments? children index options)
```

<hr class="signature-divider">

## Signature:

```roo
(simple-arguments? children index options)
```

| Arg      | Description                      |
| -------- | -------------------------------- |
| children | AST child nodes for a call form. |
| index    | First child index to inspect.    |
| options  | Rule options map.                |

### Returns:

`true` when every inspected argument is simple, otherwise `false`.

</main>
</div>
