---
layout: reference
title: proofread.rules.redundant-functions.redundant-function/skipped-function?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.redundant-functions.redundant-function.html">proofread.rules.redundant-functions.redundant-function</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="closed-over-symbol-p.html">closed-over-symbol?</a></li>
<li><a href="closed-over-wrapper-arguments-p.html">closed-over-wrapper-arguments?</a></li>
<li><a href="closed-over-wrapper-p.html">closed-over-wrapper?</a></li>
<li><a href="default-options.html">default-options</a></li>
<li><a href="defun-body-index.html">defun-body-index</a></li>
<li><a href="directly-redundant-predicate-call-p.html">directly-redundant-predicate-call?</a></li>
<li><a href="literal-node-p.html">literal-node?</a></li>
<li><a href="parameter-node-p.html">parameter-node?</a></li>
<li><a href="plain-param-names.html">plain-param-names</a></li>
<li><a href="redundant-function-p.html">redundant-function?</a></li>
<li><a href="redundant-keyword-accessor-p.html">redundant-keyword-accessor?</a></li>
<li><a class="active" href="skipped-function-p.html">skipped-function?</a></li>
<li><a href="specialized-wrapper-p.html">specialized-wrapper?</a></li>
<li><a href="unqualified-name.html">unqualified-name</a></li>
<li><a href="wrapper-arguments-p.html">wrapper-arguments?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.redundant-functions.redundant-function.html">proofread.rules.redundant-functions.redundant-function</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">skipped-function?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.redundant-functions.redundant-function/skipped-function-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.redundant-functions.redundant-function.html">proofread.rules.redundant-functions.redundant-function</a>/</div>
<h1><code>skipped-function?</code></h1>
</header>

Test whether a function name is exempt from redundant-function checks.

<hr class="signature-divider">

```roo
(skipped-function? node options)
```

<hr class="signature-divider">

## Signature:

```roo
(skipped-function? node options)
```

| Arg     | Description |
| ------- | ----------- |
| node    |             |
| options |             |

</main>
</div>
