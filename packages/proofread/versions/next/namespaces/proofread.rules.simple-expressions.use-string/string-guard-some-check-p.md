---
layout: reference
title: proofread.rules.simple-expressions.use-string/string-guard-some-check?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="some-symbol-name.html">some-symbol-name</a></li>
<li><a class="active" href="string-guard-some-check-p.html">string-guard-some-check?</a></li>
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
<span aria-current="page">string-guard-some-check?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-string/string-guard-some-check-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-string.html">proofread.rules.simple-expressions.use-string</a>/</div>
<h1><code>string-guard-some-check?</code></h1>
</header>

Test whether `some?` guards a string operation.

<hr class="signature-divider">

```roo
(string-guard-some-check? {:keys [children] :as parent} node)
```

<hr class="signature-divider">

## Signature:

```roo
(string-guard-some-check? {:keys [children] :as parent} node)
```

| Arg                           | Description                 |
| ----------------------------- | --------------------------- |
| {:keys [children] :as parent} |                             |
| node                          | Current `roo.ast` node map. |

### Returns:

`true` for `(and (some? value) (string/... value ...))`, otherwise `false`.

- `parent`: Parent `roo.ast` node map, or `nil` for root nodes.

### Returns:

`true` for `(and (some? value) (string/... value ...))`, otherwise `false`.

</main>
</div>
