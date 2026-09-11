---
layout: reference
title: proofread.rules.simple-expressions.use-string-starts-with/replacement
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.simple-expressions.use-string-starts-with.html">proofread.rules.simple-expressions.use-string-starts-with</a>
<ol>
<li><a href="candidate-p.html">candidate?</a></li>
<li><a href="case-candidate-p.html">case-candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="equality-candidate-p.html">equality-candidate?</a></li>
<li><a href="leading-substr-call-p.html">leading-substr-call?</a></li>
<li><a href="leading-substr-source.html">leading-substr-source</a></li>
<li><a class="active" href="replacement.html">replacement</a></li>
<li><a href="trailing-substr-call-p.html">trailing-substr-call?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.simple-expressions.use-string-starts-with.html">proofread.rules.simple-expressions.use-string-starts-with</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">replacement</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-string-starts-with/replacement.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.simple-expressions.use-string-starts-with.html">proofread.rules.simple-expressions.use-string-starts-with</a>/</div>
<h1><code>replacement</code></h1>
</header>

Return a source replacement string for `string/starts-with?`.

<hr class="signature-divider">

```roo
(replacement {:keys [source prefix replacement-body]})
```

<hr class="signature-divider">

## Signature:

```roo
(replacement {:keys [source prefix replacement-body]})
```

| Arg                                      | Description |
| ---------------------------------------- | ----------- |
| {:keys [source prefix replacement-body]} |             |

</main>
</div>
