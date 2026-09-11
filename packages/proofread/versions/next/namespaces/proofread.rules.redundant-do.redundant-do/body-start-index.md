---
layout: reference
title: proofread.rules.redundant-do.redundant-do/body-start-index
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>
<ol>
<li><a class="active" href="body-start-index.html">body-start-index</a></li>
<li><a href="check.html">check</a></li>
<li><a href="redundant-do-p.html">redundant-do?</a></li>
<li><a href="sole-body-form-p.html">sole-body-form?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">body-start-index</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.redundant-do.redundant-do/body-start-index.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>/</div>
<h1><code>body-start-index</code></h1>
</header>

Return the child index where a form's implicit body starts.

<hr class="signature-divider">

```roo
(body-start-index {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(body-start-index {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

</main>
</div>
