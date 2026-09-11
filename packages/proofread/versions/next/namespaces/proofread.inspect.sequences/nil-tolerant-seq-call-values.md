---
layout: reference
title: proofread.inspect.sequences/nil-tolerant-seq-call-values
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.sequences.html">proofread.inspect.sequences</a>
<ol>
<li><a href="filter-mapped-nil-results-p.html">filter-mapped-nil-results?</a></li>
<li><a href="nil-filter-p.html">nil-filter?</a></li>
<li><a href="nil-tolerant-call-values.html">nil-tolerant-call-values</a></li>
<li><a href="nil-tolerant-map-call-rules.html">nil-tolerant-map-call-rules</a></li>
<li><a href="nil-tolerant-map-call-values.html">nil-tolerant-map-call-values</a></li>
<li><a href="nil-tolerant-seq-binding-rules.html">nil-tolerant-seq-binding-rules</a></li>
<li><a href="nil-tolerant-seq-binding-values.html">nil-tolerant-seq-binding-values</a></li>
<li><a href="nil-tolerant-seq-call-rules.html">nil-tolerant-seq-call-rules</a></li>
<li><a class="active" href="nil-tolerant-seq-call-values.html">nil-tolerant-seq-call-values</a></li>
<li><a href="not-nil-param-p.html">not-nil-param?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.sequences.html">proofread.inspect.sequences</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">nil-tolerant-seq-call-values</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.sequences/nil-tolerant-seq-call-values.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.sequences.html">proofread.inspect.sequences</a>/</div>
<h1><code>nil-tolerant-seq-call-values</code></h1>
</header>

Return call arguments that treat nil as an empty sequence.

<hr class="signature-divider">

```roo
(nil-tolerant-seq-call-values node)
```

<hr class="signature-divider">

## Signature:

```roo
(nil-tolerant-seq-call-values node)
```

| Arg  | Description           |
| ---- | --------------------- |
| node | A `roo.ast` node map. |

### Returns:

Sequence argument nodes for which `nil` and `[]` have equivalent behavior,
or `nil` when `node` is not such a call.

</main>
</div>
