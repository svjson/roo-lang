---
layout: reference
title: proofread.rules.simple-expressions.redundant-map-check/lookup-with-same-fallback?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.redundant-map-check.html">proofread.rules.simple-expressions.redundant-map-check</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="guarded-lookup-branch-p.html">guarded-lookup-branch?</a></li>
<li><a href="lookup-access-p.html">lookup-access?</a></li>
<li><a href="lookup-comparison-p.html">lookup-comparison?</a></li>
<li><a class="active" href="lookup-with-same-fallback-p.html">lookup-with-same-fallback?</a></li>
<li><a href="map-check-p.html">map-check?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.redundant-map-check.html">proofread.rules.simple-expressions.redundant-map-check</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup-with-same-fallback?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-map-check/lookup-with-same-fallback-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.redundant-map-check.html">proofread.rules.simple-expressions.redundant-map-check</a>/</div>
<h1><code>lookup-with-same-fallback?</code></h1>
</header>

Test whether an `or` branch reads a lookup with the else fallback.

<hr class="signature-divider">

```roo
(lookup-with-same-fallback? {:keys [children] :as node} target-name fallback)
```

<hr class="signature-divider">

## Signature:

```roo
(lookup-with-same-fallback? {:keys [children] :as node} target-name fallback)
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |
| target-name                 |             |
| fallback                    |             |

</main>
</div>
