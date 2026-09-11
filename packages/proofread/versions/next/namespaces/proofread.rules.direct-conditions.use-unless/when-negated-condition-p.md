---
layout: reference
title: proofread.rules.direct-conditions.use-unless/when-negated-condition?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-unless.html">proofread.rules.direct-conditions.use-unless</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="if-empty-then-branch-p.html">if-empty-then-branch?</a></li>
<li><a href="replacement.html">replacement</a></li>
<li><a class="active" href="when-negated-condition-p.html">when-negated-condition?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.direct-conditions.use-unless.html">proofread.rules.direct-conditions.use-unless</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">when-negated-condition?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-unless/when-negated-condition-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-unless.html">proofread.rules.direct-conditions.use-unless</a>/</div>
<h1><code>when-negated-condition?</code></h1>
</header>

Test whether a `when` form negates its condition.

<hr class="signature-divider">

```roo
(when-negated-condition? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(when-negated-condition? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
