---
layout: reference
title: proof.reporter.common/pad-status-word
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.common.html">proof.reporter.common</a>
<ol>
<li><a href="dark-red.html">dark-red</a></li>
<li><a href="duration-args.html">duration-args</a></li>
<li><a href="highlight-test-name.html">highlight-test-name</a></li>
<li><a class="active" href="pad-status-word.html">pad-status-word</a></li>
<li><a href="padded-status-label.html">padded-status-label</a></li>
<li><a href="status-column-width.html">status-column-width</a></li>
<li><a href="status-label.html">status-label</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.reporter.common.html">proof.reporter.common</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">pad-status-word</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.common/pad-status-word.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.common.html">proof.reporter.common</a>/</div>
<h1><code>pad-status-word</code></h1>
</header>

Right-justify WORD to `status-column-width` with leading spaces.

<hr class="signature-divider">

```roo
(pad-status-word word)
```

<hr class="signature-divider">

## Signature:

```roo
(pad-status-word word)
```

| Arg  | Description |
| ---- | ----------- |
| word |             |

</main>
</div>
