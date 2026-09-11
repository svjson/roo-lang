---
layout: reference
title: cli-trooper.parse.command/select
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<ol>
<li><a href="command-flag.html">command-flag</a></li>
<li><a href="pending-defaults.html">pending-defaults</a></li>
<li><a class="active" href="select.html">select</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">select</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.command/select.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>/</div>
<h1><code>select</code></h1>
</header>

Resolve positional or command-shaped flag selection from an argument sequence.

<hr class="signature-divider">

```roo
(select config args)
```

<hr class="signature-divider">

## Signature:

```roo
(select config args)
```

| Arg    | Description                                   |
| ------ | --------------------------------------------- |
| config | A normalized cli-trooper configuration model. |
| args   | The CLI argument sequence to inspect.         |

### Returns:

A map describing the selected command path, source specification, argument
transition indexes, and declared command names, or a map containing selection
`:errors` when no executable state is selected. The empty command path `[]`
selects an executable root. A command-shaped flag attached to a child extends
the command path; an inherited root flag selects its root command identity.

</main>
</div>
