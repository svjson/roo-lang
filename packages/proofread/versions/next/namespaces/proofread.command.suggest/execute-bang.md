---
layout: reference
title: proofread.command.suggest/execute!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.command.suggest.html">proofread.command.suggest</a>
<ol>
<li><a href="checked-diagnostics.html">checked-diagnostics</a></li>
<li><a href="command.html">command</a></li>
<li><a class="active" href="execute-bang.html">execute!</a></li>
<li><a href="inner-diagnostic-before-p.html">inner-diagnostic-before?</a></li>
<li><a href="parse-error-diagnostic.html">parse-error-diagnostic</a></li>
<li><a href="parse-positive-int.html">parse-positive-int</a></li>
<li><a href="position-before-or-at-p.html">position-before-or-at?</a></li>
<li><a href="position-before-p.html">position-before?</a></li>
<li><a href="suggestions-at.html">suggestions-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.command.suggest.html">proofread.command.suggest</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">execute!</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.command.suggest/execute-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.command.suggest.html">proofread.command.suggest</a>/</div>
<h1><code>execute!</code></h1>
</header>

Print suggestions at a file position from parsed command options.

<hr class="signature-divider">

```roo
(execute! context options)
```

<hr class="signature-divider">

## Signature:

```roo
(execute! context options)
```

| Arg     | Description                                                |
| ------- | ---------------------------------------------------------- |
| context |                                                            |
| options | Parsed options containing `:file`, `:line`, and `:column`. |

### Returns:

Matching diagnostics with structured suggestions.

- `_context`: The cli-trooper invocation context.

### Returns:

Matching diagnostics with structured suggestions.

</main>
</div>
