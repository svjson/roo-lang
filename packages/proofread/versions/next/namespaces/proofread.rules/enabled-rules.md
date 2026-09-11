---
layout: reference
title: proofread.rules/enabled-rules
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.html">proofread.rules</a>
<ol>
<li><a href="categories.html">categories</a></li>
<li><a href="check.html">check</a></li>
<li><a href="check-file-with-config.html">check-file-with-config</a></li>
<li><a href="check-with-config.html">check-with-config</a></li>
<li><a class="active" href="enabled-rules.html">enabled-rules</a></li>
<li><a href="rules.html">rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.html">proofread.rules</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">enabled-rules</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules/enabled-rules.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.html">proofread.rules</a>/</div>
<h1><code>enabled-rules</code></h1>
</header>

Return rule descriptors enabled by config.

<hr class="signature-divider">

```roo
(enabled-rules {:keys [rules] :as active-config} rule-descriptors)
```

<hr class="signature-divider">

## Signature:

```roo
(enabled-rules {:keys [rules] :as active-config} rule-descriptors)
```

| Arg                               | Description           |
| --------------------------------- | --------------------- |
| {:keys [rules] :as active-config} |                       |
| rule-descriptors                  | Rule descriptor maps. |

### Returns:

Rule descriptors not disabled by config.

- `active-config`: Effective proofread config.

### Returns:

Rule descriptors not disabled by config.

</main>
</div>
