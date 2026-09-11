---
layout: reference
title: cli-trooper.invoke/command-flag-execution
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.invoke.html">cli-trooper.invoke</a>
<ol>
<li><a class="active" href="command-flag-execution.html">command-flag-execution</a></li>
<li><a href="resolve-execution.html">resolve-execution</a></li>
<li><a href="run.html">run</a></li>
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
<span aria-current="page">command-flag-execution</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.invoke/command-flag-execution.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.invoke.html">cli-trooper.invoke</a>/</div>
<h1><code>command-flag-execution</code></h1>
</header>

Resolve a command-shaped flag execution function by command identity.

<hr class="signature-divider">

```roo
(command-flag-execution flags command)
```

<hr class="signature-divider">

## Signature:

```roo
(command-flag-execution flags command)
```

| Arg     | Description                                             |
| ------- | ------------------------------------------------------- |
| flags   | A canonical flag map.                                   |
| command | The command identity selected by a command-shaped flag. |

### Returns:

The declared `:execute!` function, or `nil` when no matching command-shaped
flag carries one.

</main>
</div>
