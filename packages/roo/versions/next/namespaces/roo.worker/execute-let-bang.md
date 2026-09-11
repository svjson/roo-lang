---
layout: reference
title: roo.worker/execute-let!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.worker.html">roo.worker</a>
<ol>
<li><a href="collect-bang.html">collect!</a></li>
<li><a href="create-bang.html">create!</a></li>
<li><a class="active" href="execute-let-bang.html">execute-let!</a></li>
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
<span aria-current="page">execute-let!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.worker/execute-let-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.worker.html">roo.worker</a>/</div>
<h1><code>execute-let!</code></h1>
<div class="symbol-metadata"><span>Since <code>0.1.0</code></span></div>
</header>

Schedule worker-side evaluation with parent-evaluated bindings.

<hr class="signature-divider">

```roo
(execute-let! identity bindings body...)
```

<hr class="signature-divider">

Binding expressions are evaluated sequentially in the parent runtime and
copied together into a job-local scope in the named worker. The body is
retained as source-accurate AST, lowered against the worker runtime, and
evaluated there. Names not introduced by the binding vector therefore
resolve only in the worker runtime.

### Usage:

```roo
(roo.worker/execute-let! :my-app/worker
[data (load-data)]
(process data))
=> #<roo.worker/execution :my-app/worker 1>
```

<hr class="signature-divider">

## Signature:

```roo
(execute-let! identity bindings body...)
```

| Arg      | Description                                                    |
| -------- | -------------------------------------------------------------- |
| identity | Keyword identifying the worker in the current runtime.         |
| bindings | Sequential parent-side binding and value-expression pairs.     |
| body...  | Forms lowered and evaluated in the worker with those bindings. |

### Returns:

An execution handle, as accepted by `poll!` and `collect!`.

</main>
</div>
