---
layout: reference
title: cli-trooper.parse.command/command-flag
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>
<ol>
<li><a class="active" href="command-flag.html">command-flag</a></li>
<li><a href="pending-defaults.html">pending-defaults</a></li>
<li><a href="select.html">select</a></li>
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
<span aria-current="page">command-flag</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.command/command-flag.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.command.html">cli-trooper.parse.command</a>/</div>
<h1><code>command-flag</code></h1>
</header>

Find a command-shaped flag declaration matching one argument.

<hr class="signature-divider">

```roo
(command-flag flags arg)
```

<hr class="signature-divider">

An assigned long spelling also matches so command selection can report that
the terminal flag does not accept a value.

<hr class="signature-divider">

## Signature:

```roo
(command-flag flags arg)
```

| Arg   | Description                       |
| ----- | --------------------------------- |
| flags | An applicable canonical flag map. |
| arg   | The raw argument to match.        |

### Returns:

A map containing the matched canonical `:names` and `:spec`. An assigned
spelling also carries its canonical `:argument` and `:assigned? true`; an
unmatched argument returns `nil`.

</main>
</div>
