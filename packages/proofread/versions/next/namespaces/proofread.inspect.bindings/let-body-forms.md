---
layout: reference
title: proofread.inspect.bindings/let-body-forms
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.bindings.html">proofread.inspect.bindings</a>
<ol>
<li><a href="binding-use-count.html">binding-use-count</a></li>
<li><a href="body-use-count.html">body-use-count</a></li>
<li><a href="first-single-use-simple-binding.html">first-single-use-simple-binding</a></li>
<li><a href="later-binding-use-count.html">later-binding-use-count</a></li>
<li><a href="let-bindings.html">let-bindings</a></li>
<li><a class="active" href="let-body-forms.html">let-body-forms</a></li>
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
<span aria-current="page">let-body-forms</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.bindings/let-body-forms.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.bindings.html">proofread.inspect.bindings</a>/</div>
<h1><code>let-body-forms</code></h1>
</header>

Return body children for a `let` form.

<hr class="signature-divider">

```roo
(let-body-forms {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(let-body-forms {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

A sequence of `let` body forms, otherwise `nil`.

- `node`: A `roo.ast` node map.

### Returns:

A sequence of `let` body forms, otherwise `nil`.

</main>
</div>
