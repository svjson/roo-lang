---
layout: reference
title: cli-trooper.config.command/mixed-flags
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.command.html">cli-trooper.config.command</a>
<ol>
<li><a class="active" href="mixed-flags.html">mixed-flags</a></li>
<li><a href="normalize.html">normalize</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.command.html">cli-trooper.config.command</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">mixed-flags</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.command/mixed-flags.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.command.html">cli-trooper.config.command</a>/</div>
<h1><code>mixed-flags</code></h1>
</header>

Compose inherited and command-local flag declarations.

<hr class="signature-divider">

```roo
(mixed-flags config {:keys [mixin flags]})
```

<hr class="signature-divider">

## Signature:

```roo
(mixed-flags config {:keys [mixin flags]})
```

| Arg                   | Description                                                                                                                                                                                       |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| config                | The supplied application configuration containing `:mixins`.<br>- `mixin`: The command's optional mixin identifier or sequence of identifiers.<br>- `flags`: The command-local supplied flag map. |
| {:keys [mixin flags]} |                                                                                                                                                                                                   |

### Returns:

A supplied-shape flag map with mixins applied in declaration order and local
flags applied last.

</main>
</div>
