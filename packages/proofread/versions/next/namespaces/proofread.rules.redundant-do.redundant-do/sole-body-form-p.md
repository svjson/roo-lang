---
layout: reference
title: proofread.rules.redundant-do.redundant-do/sole-body-form?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>
<ol>
<li><a href="body-start-index.html">body-start-index</a></li>
<li><a href="check.html">check</a></li>
<li><a href="redundant-do-p.html">redundant-do?</a></li>
<li><a class="active" href="sole-body-form-p.html">sole-body-form?</a></li>
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
<span aria-current="page">sole-body-form?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.redundant-do.redundant-do/sole-body-form-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.redundant-do.redundant-do.html">proofread.rules.redundant-do.redundant-do</a>/</div>
<h1><code>sole-body-form?</code></h1>
</header>

Test whether `node` is the only form in `parent`'s implicit body.

<hr class="signature-divider">

```roo
(sole-body-form? {:keys [children] :as parent} node)
```

<hr class="signature-divider">

## Signature:

```roo
(sole-body-form? {:keys [children] :as parent} node)
```

| Arg                           | Description |
| ----------------------------- | ----------- |
| {:keys [children] :as parent} |             |
| node                          |             |

</main>
</div>
