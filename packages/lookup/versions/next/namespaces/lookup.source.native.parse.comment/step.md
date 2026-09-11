---
layout: reference
title: lookup.source.native.parse.comment/step
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../lookup.source.native.parse.comment.html">lookup.source.native.parse.comment</a>
<ol>
<li><a href="annotation.html">annotation</a></li>
<li><a href="append-annotation-text.html">append-annotation-text</a></li>
<li><a href="append-header.html">append-header</a></li>
<li><a href="append-target-block.html">append-target-block</a></li>
<li><a href="arg-row.html">arg-row</a></li>
<li><a href="code-language.html">code-language</a></li>
<li><a href="directive-value.html">directive-value</a></li>
<li><a href="finish-block.html">finish-block</a></li>
<li><a href="finish-code.html">finish-code</a></li>
<li><a href="finish-text.html">finish-text</a></li>
<li><a href="header-p.html">header?</a></li>
<li><a href="identity.html">identity</a></li>
<li><a href="line-identity.html">line-identity</a></li>
<li><a href="lines.html">lines</a></li>
<li><a href="namespace-annotation.html">namespace-annotation</a></li>
<li><a href="param-directive.html">param-directive</a></li>
<li><a href="parse.html">parse</a></li>
<li><a href="return-text.html">return-text</a></li>
<li><a href="reuse-single-return.html">reuse-single-return</a></li>
<li><a href="start-args.html">start-args</a></li>
<li><a href="start-code.html">start-code</a></li>
<li><a href="start-param.html">start-param</a></li>
<li><a href="start-return.html">start-return</a></li>
<li><a class="active" href="step.html">step</a></li>
<li><a href="strip-first-prefix.html">strip-first-prefix</a></li>
<li><a href="strip-line.html">strip-line</a></li>
<li><a href="strip-suffix.html">strip-suffix</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../lookup.source.native.parse.comment.html">lookup.source.native.parse.comment</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">step</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/lookup.source.native.parse.comment/step.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../lookup.source.native.parse.comment.html">lookup.source.native.parse.comment</a>/</div>
<h1><code>step</code></h1>
</header>

<hr class="signature-divider">

```roo
(step {:keys [code boundary? target] :as state} line)
```

<hr class="signature-divider">

## Signature:

```roo
(step {:keys [code boundary? target] :as state} line)
```

| Arg                                       | Description |
| ----------------------------------------- | ----------- |
| {:keys [code boundary? target] :as state} |             |
| line                                      |             |

</main>
</div>
