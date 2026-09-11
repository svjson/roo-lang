---
layout: reference
title: roo.worker/poll!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.worker.html">roo.worker</a>
<ol>
<li><a href="collect-bang.html">collect!</a></li>
<li><a href="create-bang.html">create!</a></li>
<li><a href="execute-let-bang.html">execute-let!</a></li>
<li><a href="invoke-bang.html">invoke!</a></li>
<li><a class="active" href="poll-bang.html">poll!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.worker.html">roo.worker</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">poll!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.worker/poll-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.worker.html">roo.worker</a>/</div>
<h1><code>poll!</code></h1>
<div class="symbol-metadata"><span>Since <code>0.1.0</code></span></div>
</header>

Return the current status of a worker execution.

<hr class="signature-divider">

```roo
(poll! execution options)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.worker/poll! execution)
=> {:status :queued}
(roo.worker/poll! execution {:timeout-ms 80})
=> {:status :running}
```

<hr class="signature-divider">

## Signature:

```roo
(poll! execution options)
```

| Arg       | Description                                             |
| --------- | ------------------------------------------------------- |
| execution | Execution handle returned by a worker operation.        |
| options   | Optional map with a non-negative integer `:timeout-ms`. |

### Returns:

A report map whose `:status` is one of `:queued`, `:running`,
`:succeeded`, or `:failed`. Missing options or `:timeout-ms` default to
zero and return immediately.

</main>
</div>
