---
layout: reference
title: proofread.inspect.bindings/later-binding-use-count
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.bindings.html">proofread.inspect.bindings</a>
<ol>
<li><a href="binding-use-count.html">binding-use-count</a></li>
<li><a href="body-use-count.html">body-use-count</a></li>
<li><a href="first-single-use-simple-binding.html">first-single-use-simple-binding</a></li>
<li><a class="active" href="later-binding-use-count.html">later-binding-use-count</a></li>
<li><a href="let-bindings.html">let-bindings</a></li>
<li><a href="let-body-forms.html">let-body-forms</a></li>
<li><a href="single-use-simple-binding-p.html">single-use-simple-binding?</a></li>
<li><a href="symbol-use-count.html">symbol-use-count</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.bindings.html">proofread.inspect.bindings</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">later-binding-use-count</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.bindings/later-binding-use-count.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.bindings.html">proofread.inspect.bindings</a>/</div>
<h1><code>later-binding-use-count</code></h1>
</header>

Count symbol uses in later binding values.

<hr class="signature-divider">

```roo
(later-binding-use-count name bindings value-index)
```

<hr class="signature-divider">

## Signature:

```roo
(later-binding-use-count name bindings value-index)
```

| Arg         | Description                           |
| ----------- | ------------------------------------- |
| name        | Symbol name to count.                 |
| bindings    | Let binding vector children.          |
| value-index | First binding value index to inspect. |

### Returns:

The number of references to `name` in later binding values.

</main>
</div>
