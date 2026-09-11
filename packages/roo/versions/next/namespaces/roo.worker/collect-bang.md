---
layout: reference
title: roo.worker/collect!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.worker.html">roo.worker</a>
<ol>
<li><a class="active" href="collect-bang.html">collect!</a></li>
<li><a href="create-bang.html">create!</a></li>
<li><a href="execute-let-bang.html">execute-let!</a></li>
<li><a href="invoke-bang.html">invoke!</a></li>
<li><a href="poll-bang.html">poll!</a></li>
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
<span aria-current="page">collect!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.worker/collect-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.worker.html">roo.worker</a>/</div>
<h1><code>collect!</code></h1>
<div class="symbol-metadata"><span>Since <code>0.1.0</code></span></div>
</header>

Consume and return a completed worker execution's result.

<hr class="signature-divider">

```roo
(collect! execution)
```

<hr class="signature-divider">

Collection removes the execution and its retained result from the worker.
Collecting an unfinished, already collected, or unknown execution is an
error. A failed execution raises its failure and is likewise removed.

### Usage:

```roo
(roo.worker/collect! execution)
=> {:answer 42}
```

<hr class="signature-divider">

## Signature:

```roo
(collect! execution)
```

| Arg       | Description                                                |
| --------- | ---------------------------------------------------------- |
| execution | Completed execution handle returned by a worker operation. |

### Returns:

The unwrapped worker execution result.

</main>
</div>
