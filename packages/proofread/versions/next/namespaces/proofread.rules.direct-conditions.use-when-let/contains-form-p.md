---
layout: reference
title: proofread.rules.direct-conditions.use-when-let/contains-form?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.direct-conditions.use-when-let.html">proofread.rules.direct-conditions.use-when-let</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="contains-form-p.html">contains-form?</a></li>
<li><a href="let-candidate-p.html">let-candidate?</a></li>
<li><a href="nil-else-if-let-p.html">nil-else-if-let?</a></li>
<li><a href="repeated-keyword-lookup-p.html">repeated-keyword-lookup?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.direct-conditions.use-when-let.html">proofread.rules.direct-conditions.use-when-let</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">contains-form?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.direct-conditions.use-when-let/contains-form-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.direct-conditions.use-when-let.html">proofread.rules.direct-conditions.use-when-let</a>/</div>
<h1><code>contains-form?</code></h1>
</header>

Test whether an AST subtree contains a matching form.

<hr class="signature-divider">

```roo
(contains-form? node target)
```

<hr class="signature-divider">

## Signature:

```roo
(contains-form? node target)
```

| Arg    | Description |
| ------ | ----------- |
| node   |             |
| target |             |

</main>
</div>
