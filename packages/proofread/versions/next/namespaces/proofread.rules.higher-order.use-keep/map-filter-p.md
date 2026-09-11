---
layout: reference
title: proofread.rules.higher-order.use-keep/map-filter?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.higher-order.use-keep.html">proofread.rules.higher-order.use-keep</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a class="active" href="map-filter-p.html">map-filter?</a></li>
<li><a href="map-with-conditional-body-p.html">map-with-conditional-body?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.higher-order.use-keep.html">proofread.rules.higher-order.use-keep</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">map-filter?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.higher-order.use-keep/map-filter-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.higher-order.use-keep.html">proofread.rules.higher-order.use-keep</a>/</div>
<h1><code>map-filter?</code></h1>
</header>

Test whether a map consumes a filtered sequence.

<hr class="signature-divider">

```roo
(map-filter? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(map-filter? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
