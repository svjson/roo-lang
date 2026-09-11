---
layout: reference
title: proofread.rules.simple-expressions.use-string/some-symbol-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="some-symbol-name.html">some-symbol-name</a></li>
<li><a href="string-guard-some-check-p.html">string-guard-some-check?</a></li>
<li><a href="string-operation-on-p.html">string-operation-on?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">some-symbol-name</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-string/some-symbol-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>/</div>
<h1><code>some-symbol-name</code></h1>
</header>

Return the checked symbol name from a `some?` form.

<hr class="signature-divider">

```roo
(some-symbol-name {:keys [children] :as node})
```

<hr class="signature-divider">

## Signature:

```roo
(some-symbol-name {:keys [children] :as node})
```

| Arg                         | Description |
| --------------------------- | ----------- |
| {:keys [children] :as node} |             |

### Returns:

The symbol name for `(some? name)`, otherwise `nil`.

- `node`: A `roo.ast` node map.

### Returns:

The symbol name for `(some? name)`, otherwise `nil`.

</main>
</div>
