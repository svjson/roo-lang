---
layout: reference
title: proofread.rules.direct-conditions.use-cond/candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-cond.html">proofread.rules.direct-conditions.use-cond</a>
<ol>
<li><a class="active" href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="value-if-chain-p.html">value-if-chain?</a></li>
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
<span aria-current="page">candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-cond/candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-cond.html">proofread.rules.direct-conditions.use-cond</a>/</div>
<h1><code>candidate?</code></h1>
</header>

Test whether an `if` chain should be `cond`.

<hr class="signature-divider">

```roo
(candidate? node)
```

<hr class="signature-divider">

## Signature:

```roo
(candidate? node)
```

| Arg  | Description |
| ---- | ----------- |
| node |             |

</main>
</div>
