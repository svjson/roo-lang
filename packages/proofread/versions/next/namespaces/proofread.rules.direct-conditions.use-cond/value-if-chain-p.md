---
layout: reference
title: proofread.rules.direct-conditions.use-cond/value-if-chain?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-cond.html">proofread.rules.direct-conditions.use-cond</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a class="active" href="value-if-chain-p.html">value-if-chain?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.direct-conditions.use-cond.html">proofread.rules.direct-conditions.use-cond</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">value-if-chain?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-cond/value-if-chain-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-cond.html">proofread.rules.direct-conditions.use-cond</a>/</div>
<h1><code>value-if-chain?</code></h1>
</header>

Test whether an `if` chain has meaningful branch values.

<hr class="signature-divider">

```roo
(value-if-chain? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(value-if-chain? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
