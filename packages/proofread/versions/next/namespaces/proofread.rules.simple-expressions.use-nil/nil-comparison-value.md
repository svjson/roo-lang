---
layout: reference
title: proofread.rules.simple-expressions.use-nil/nil-comparison-value
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-nil.html">proofread.rules.simple-expressions.use-nil</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="nil-comparison-value.html">nil-comparison-value</a></li>
<li><a href="nil-comparison-p.html">nil-comparison?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-nil.html">proofread.rules.simple-expressions.use-nil</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">nil-comparison-value</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-nil/nil-comparison-value.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-nil.html">proofread.rules.simple-expressions.use-nil</a>/</div>
<h1><code>nil-comparison-value</code></h1>
</header>

Return the non-nil side of a nil comparison.

<hr class="signature-divider">

```roo
(nil-comparison-value {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(nil-comparison-value {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
