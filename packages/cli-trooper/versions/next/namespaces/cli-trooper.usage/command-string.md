---
layout: reference
title: cli-trooper.usage/command-string
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.usage.html">cli-trooper.usage</a>
<ol>
<li><a class="active" href="command-string.html">command-string</a></li>
<li><a href="format-flag.html">format-flag</a></li>
<li><a href="format-flag-arg.html">format-flag-arg</a></li>
<li><a href="format-flag-name.html">format-flag-name</a></li>
<li><a href="format-positional.html">format-positional</a></li>
<li><a href="usage-string.html">usage-string</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.usage.html">cli-trooper.usage</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">command-string</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.usage/command-string.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.usage.html">cli-trooper.usage</a>/</div>
<h1><code>command-string</code></h1>
</header>

Build the one-line usage form for a command.

<hr class="signature-divider">

```roo
(command-string application cmd)
```

<hr class="signature-divider">

## Signature:

```roo
(command-string application cmd)
```

| Arg         | Description                                                                                                  |
| ----------- | ------------------------------------------------------------------------------------------------------------ |
| application | The application name shown at the start of the line.                                                         |
| cmd         | A normalized root or command state containing positionals and flags,<br>plus `:command` for a named command. |

### Returns:

A single usage string for the root state or command.

</main>
</div>
