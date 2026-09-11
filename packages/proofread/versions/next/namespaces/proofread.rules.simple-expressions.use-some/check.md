---
layout: reference
title: proofread.rules.simple-expressions.use-some/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-some.html">proofread.rules.simple-expressions.use-some</a>
<ol>
<li><a class="active" href="check.html">check</a></li>
<li><a href="direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a href="nil-check-p.html">nil-check?</a></li>
<li><a href="not-nil-check-p.html">not-nil-check?</a></li>
<li><a href="replacement.html">replacement</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-some.html">proofread.rules.simple-expressions.use-some</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-some/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-some.html">proofread.rules.simple-expressions.use-some</a>/</div>
<h1><code>check</code></h1>
</header>

Report negated `nil?` checks.

<hr class="signature-divider">

```roo
(check parent node options)
```

<hr class="signature-divider">

## Signature:

```roo
(check parent node options)
```

| Arg     | Description                                                                                |
| ------- | ------------------------------------------------------------------------------------------ |
| parent  | Parent `roo.ast` node map, or `nil` for a root node.                                       |
| node    | Current `roo.ast` node map.<br>- `_options`: Ignored effective rule options map, or `nil`. |
| options |                                                                                            |

### Returns:

A diagnostic map when `node` should use `some?`, otherwise `nil`.

</main>
</div>
