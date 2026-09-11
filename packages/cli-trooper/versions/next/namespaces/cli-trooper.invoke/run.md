---
layout: reference
title: cli-trooper.invoke/run
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.invoke.html">cli-trooper.invoke</a>
<ol>
<li><a href="command-flag-execution.html">command-flag-execution</a></li>
<li><a href="resolve-execution.html">resolve-execution</a></li>
<li><a class="active" href="run.html">run</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.invoke.html">cli-trooper.invoke</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">run</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.invoke/run.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.invoke.html">cli-trooper.invoke</a>/</div>
<h1><code>run</code></h1>
</header>

Invoke the execution function selected by a successful parse result.

<hr class="signature-divider">

```roo
(run config context parsed)
```

<hr class="signature-divider">

The function receives the caller context augmented with the canonical model
under `:cli-trooper/config` and the selected identity under
`:cli-trooper/command`, followed by the resolved options map. Parse failures
are returned unchanged and never execute a command.

<hr class="signature-divider">

## Signature:

```roo
(run config context parsed)
```

| Arg     | Description                                  |
| ------- | -------------------------------------------- |
| config  | A canonical cli-trooper configuration model. |
| context | The CLI invocation context.                  |
| parsed  | A cli-trooper parse result.                  |

### Returns:

The parse result with the execution value under `:result`, or with a
`:missing-command-execution` diagnostic when the selected command has no
`:execute!` function.

</main>
</div>
