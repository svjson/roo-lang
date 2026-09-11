---
layout: reference
title: lookup.source.docstring.parse.content/step
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.docstring.parse.content.html">lookup.source.docstring.parse.content</a>
<ol>
<li><a href="section-targets.html">SECTION-TARGETS</a></li>
<li><a href="append-header.html">append-header</a></li>
<li><a href="append-metadata.html">append-metadata</a></li>
<li><a href="append-target-block.html">append-target-block</a></li>
<li><a href="arg-declaration.html">arg-declaration</a></li>
<li><a href="fence-language.html">fence-language</a></li>
<li><a href="finish-block.html">finish-block</a></li>
<li><a href="finish-fence.html">finish-fence</a></li>
<li><a href="finish-text.html">finish-text</a></li>
<li><a href="header-p.html">header?</a></li>
<li><a href="parse.html">parse</a></li>
<li><a href="start-fence.html">start-fence</a></li>
<li><a class="active" href="step.html">step</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.source.docstring.parse.content.html">lookup.source.docstring.parse.content</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">step</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source.docstring.parse.content/step.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.docstring.parse.content.html">lookup.source.docstring.parse.content</a>/</div>
<h1><code>step</code></h1>
</header>

<hr class="signature-divider">

```roo
(step {:keys [fence target boundary?] :as state} line)
```

<hr class="signature-divider">

## Signature:

```roo
(step {:keys [fence target boundary?] :as state} line)
```

| Arg                                        | Description |
| ------------------------------------------ | ----------- |
| {:keys [fence target boundary?] :as state} |             |
| line                                       |             |

</main>
</div>
