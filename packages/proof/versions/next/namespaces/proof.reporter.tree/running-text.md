---
layout: reference
title: proof.reporter.tree/running-text
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.tree.html">proof.reporter.tree</a>
<ol>
<li><a href="add-pending-widgets-bang.html">add-pending-widgets!</a></li>
<li><a href="begin-test.html">begin-test</a></li>
<li><a href="bullet-for-index.html">bullet-for-index</a></li>
<li><a href="end-test.html">end-test</a></li>
<li><a href="make.html">make</a></li>
<li><a href="pending-text.html">pending-text</a></li>
<li><a href="result-text.html">result-text</a></li>
<li><a class="active" href="running-text.html">running-text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.reporter.tree.html">proof.reporter.tree</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">running-text</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.tree/running-text.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.tree.html">proof.reporter.tree</a>/</div>
<h1><code>running-text</code></h1>
</header>

Render TEST's in-progress line: BULLET, a grayed RUNNING status word,
and TEST's name.

<hr class="signature-divider">

```roo
(running-text bullet test)
```

<hr class="signature-divider">

## Signature:

```roo
(running-text bullet test)
```

| Arg    | Description |
| ------ | ----------- |
| bullet |             |
| test   |             |

</main>
</div>
