---
layout: reference
title: spool/wrap
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../spool.html">spool</a>
<ol>
<li><a href="enter-bang.html">enter!</a></li>
<li><a href="leave-bang.html">leave!</a></li>
<li><a href="record-bang.html">record!</a></li>
<li><a href="report-bang.html">report!</a></li>
<li><a href="reset-bang.html">reset!</a></li>
<li><a class="active" href="wrap.html">wrap</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">spool next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../spool.html">spool</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wrap</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/spool/wrap.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../spool.html">spool</a>/</div>
<h1><code>wrap</code></h1>
</header>

Return a function that records each call inside a timed context.

<hr class="signature-divider">

```roo
(wrap wrapped-fn {:keys [metadata frame] :as opts})
```

<hr class="signature-divider">

## Signature:

```roo
(wrap wrapped-fn {:keys [metadata frame] :as opts})
```

| Arg                               | Description                                                                                                                                                                                                                                                                                 |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| wrapped-fn                        | Function to invoke when the returned function is called.<br>- `opts`: Options map. `:frame` identifies the context, `:metadata` is<br>recorded when it opens, and `:exit-metadata` is an optional function that<br>receives the return value and produces metadata for the closing context. |
| {:keys [metadata frame] :as opts} |                                                                                                                                                                                                                                                                                             |

### Returns:

A function that forwards its arguments and returns `wrapped-fn`'s result.

</main>
</div>
