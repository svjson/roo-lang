---
layout: reference
title: proofread.rules.simple-expressions.redundant-let-rebind/all-update-steps?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.redundant-let-rebind.html">proofread.rules.simple-expressions.redundant-let-rebind</a>
<ol>
<li><a class="active" href="all-update-steps-p.html">all-update-steps?</a></li>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="cond-thread-update-keys.html">cond-thread-update-keys</a></li>
<li><a href="cond-thread-p.html">cond-thread?</a></li>
<li><a href="reads-updated-key-p.html">reads-updated-key?</a></li>
<li><a href="tests-read-updated-key-p.html">tests-read-updated-key?</a></li>
<li><a href="update-keys.html">update-keys</a></li>
<li><a href="update-step-key.html">update-step-key</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.redundant-let-rebind.html">proofread.rules.simple-expressions.redundant-let-rebind</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">all-update-steps?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-let-rebind/all-update-steps-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.redundant-let-rebind.html">proofread.rules.simple-expressions.redundant-let-rebind</a>/</div>
<h1><code>all-update-steps?</code></h1>
</header>

Test whether all `cond->` steps from `index` are `update` steps.

<hr class="signature-divider">

```roo
(all-update-steps? children index)
```

<hr class="signature-divider">

## Signature:

```roo
(all-update-steps? children index)
```

| Arg      | Description |
| -------- | ----------- |
| children |             |
| index    |             |

</main>
</div>
