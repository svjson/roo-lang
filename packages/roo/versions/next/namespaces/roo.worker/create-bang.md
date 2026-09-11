---
layout: reference
title: roo.worker/create!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.worker.html">roo.worker</a>
<ol>
<li><a href="collect-bang.html">collect!</a></li>
<li><a class="active" href="create-bang.html">create!</a></li>
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
<span aria-current="page">create!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.worker/create-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.worker.html">roo.worker</a>/</div>
<h1><code>create!</code></h1>
<div class="symbol-metadata"><span>Since <code>0.1.0</code></span></div>
</header>

Create and start a named worker owned by the current runtime.

<hr class="signature-divider">

```roo
(create! identity options)
```

<hr class="signature-divider">

The worker is registered under a runtime-local keyword identity and owns a
dedicated execution thread. The default vanilla environment has no
application namespaces, namespace loading, or file-system access inherited
from the parent. A host may register an application environment that
reconstructs its runtime bootstrap without invoking its entry point. The
optional `:autoloads` vector names additional worker-local namespaces to
load after environment initialization. The call returns only after worker
initialization and these autoloads succeed.

Creating another worker with the same identity in the current runtime is an
error. The same identity may be used independently by another runtime.

### Usage:

```roo
(roo.worker/create! :my-app/worker) => nil
(roo.worker/create! :my-app/application-worker
{:environment :application
:autoloads ["my-app.worker"]}) => nil
```

<hr class="signature-divider">

## Signature:

```roo
(create! identity options)
```

| Arg      | Description                                                      |
| -------- | ---------------------------------------------------------------- |
| identity | Keyword identifying the worker within the current runtime.       |
| options  | Optional map with `:environment` and String-vector `:autoloads`. |

### Returns:

`nil` after the worker runtime has initialized successfully.

</main>
</div>
