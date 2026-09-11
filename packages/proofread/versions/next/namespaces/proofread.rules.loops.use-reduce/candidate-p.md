---
layout: reference
title: proofread.rules.loops.use-reduce/candidate?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.loops.use-reduce.html">proofread.rules.loops.use-reduce</a>
<ol>
<li><a href="binding-names.html">binding-names</a></li>
<li><a class="active" href="candidate-p.html">candidate?</a></li>
<li><a href="check.html">check</a></li>
<li><a href="collection-loop-p.html">collection-loop?</a></li>
<li><a href="contains-mutating-set-p.html">contains-mutating-set?</a></li>
<li><a href="loop-heads.html">loop-heads</a></li>
<li><a href="loop-mutates-bound-name-p.html">loop-mutates-bound-name?</a></li>
<li><a href="mutates-bound-name-p.html">mutates-bound-name?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.loops.use-reduce.html">proofread.rules.loops.use-reduce</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">candidate?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.loops.use-reduce/candidate-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.loops.use-reduce.html">proofread.rules.loops.use-reduce</a>/</div>
<h1><code>candidate?</code></h1>
</header>

Test whether a `let` contains loop-carried mutable state.

<hr class="signature-divider">

```roo
(candidate? node)
```

<hr class="signature-divider">

## Signature:

```roo
(candidate? node)
```

| Arg  | Description |
| ---- | ----------- |
| node |             |

</main>
</div>
