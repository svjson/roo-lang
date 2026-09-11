---
layout: reference
title: proofread.rules.simple-expressions.use-clamp/nested-clamp-call?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-clamp.html">proofread.rules.simple-expressions.use-clamp</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="nested-clamp-call-p.html">nested-clamp-call?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-clamp.html">proofread.rules.simple-expressions.use-clamp</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">nested-clamp-call?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-clamp/nested-clamp-call-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-clamp.html">proofread.rules.simple-expressions.use-clamp</a>/</div>
<h1><code>nested-clamp-call?</code></h1>
</header>

Test whether a `min` or `max` call wraps the opposite operation.

<hr class="signature-divider">

```roo
(nested-clamp-call? {:keys [children] :as node} outer inner)
```

<hr class="signature-divider">

## Signature:

```roo
(nested-clamp-call? {:keys [children] :as node} outer inner)
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |
| outer                       |             |
| inner                       |             |

</main>
</div>
