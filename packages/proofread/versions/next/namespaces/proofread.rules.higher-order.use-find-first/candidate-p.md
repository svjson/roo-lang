---
layout: reference
title: proofread.rules.higher-order.use-find-first/candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.higher-order.use-find-first.html">proofread.rules.higher-order.use-find-first</a>
<ol>
<li><a class="active" href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.higher-order.use-find-first.html">proofread.rules.higher-order.use-find-first</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.higher-order.use-find-first/candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.higher-order.use-find-first.html">proofread.rules.higher-order.use-find-first</a>/</div>
<h1><code>candidate?</code></h1>
</header>

Test whether an AST node can use `find-first`.

<hr class="signature-divider">

```roo
(candidate? {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(candidate? {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
