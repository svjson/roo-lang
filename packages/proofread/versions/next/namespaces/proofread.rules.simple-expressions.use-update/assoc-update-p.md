---
layout: reference
title: proofread.rules.simple-expressions.use-update/assoc-update?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>
<ol>
<li><a class="active" href="assoc-update-p.html">assoc-update?</a></li>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="cond-thread-assoc-update-p.html">cond-thread-assoc-update?</a></li>
<li><a href="cond-thread-step-p.html">cond-thread-step?</a></li>
<li><a href="same-key-lookup-p.html">same-key-lookup?</a></li>
<li><a href="update-value-p.html">update-value?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">assoc-update?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-update/assoc-update-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-update.html">proofread.rules.simple-expressions.use-update</a>/</div>
<h1><code>assoc-update?</code></h1>
</header>

Test whether an `assoc` rebuilds a key from its current value.

<hr class="signature-divider">

```roo
(assoc-update? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(assoc-update? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
