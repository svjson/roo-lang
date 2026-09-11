---
layout: reference
title: proofread.rules.simple-expressions.use-empty/count-form-argument
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="count-form-argument.html">count-form-argument</a></li>
<li><a href="empty-count-comparison-p.html">empty-count-comparison?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">count-form-argument</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-empty/count-form-argument.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-empty.html">proofread.rules.simple-expressions.use-empty</a>/</div>
<h1><code>count-form-argument</code></h1>
</header>

Return the argument inside a `count` call in a count/zero comparison.

<hr class="signature-divider">

```roo
(count-form-argument {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(count-form-argument {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
