---
layout: reference
title: proofread.rules.simple-expressions.use-not-empty/direct-unless-candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="count-empty-arg.html">count-empty-arg</a></li>
<li><a class="active" href="direct-unless-candidate-p.html">direct-unless-candidate?</a></li>
<li><a href="negated-empty-arg.html">negated-empty-arg</a></li>
<li><a href="not-empty-count-comparison-p.html">not-empty-count-comparison?</a></li>
<li><a href="not-empty-negation-p.html">not-empty-negation?</a></li>
<li><a href="replacement.html">replacement</a></li>
<li><a href="source-argument.html">source-argument</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">direct-unless-candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-not-empty/direct-unless-candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-not-empty.html">proofread.rules.simple-expressions.use-not-empty</a>/</div>
<h1><code>direct-unless-candidate?</code></h1>
</header>

Test whether this negated expression should be handled by `use-unless` instead.

<hr class="signature-divider">

```roo
(direct-unless-candidate? parent node)
```

<hr class="signature-divider">

## Signature:

```roo
(direct-unless-candidate? parent node)
```

| Arg    | Description |
| ------ | ----------- |
| parent |             |
| node   |             |

</main>
</div>
