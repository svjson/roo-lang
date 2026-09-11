---
layout: reference
title: cli-trooper.usage
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.usage.html">cli-trooper.usage</a>
<ol>
<li><a href="cli-trooper.usage/command-string.html">command-string</a></li>
<li><a href="cli-trooper.usage/format-flag.html">format-flag</a></li>
<li><a href="cli-trooper.usage/format-flag-arg.html">format-flag-arg</a></li>
<li><a href="cli-trooper.usage/format-flag-name.html">format-flag-name</a></li>
<li><a href="cli-trooper.usage/format-positional.html">format-positional</a></li>
<li><a href="cli-trooper.usage/usage-string.html">usage-string</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.usage</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.usage.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.usage

| Symbol                                                          | Summary                                                     |
| --------------------------------------------------------------- | ----------------------------------------------------------- |
| [`command-string`](cli-trooper.usage/command-string.html)       | Build the one-line usage form for a command.                |
| [`format-flag`](cli-trooper.usage/format-flag.html)             | Format a complete flag declaration for usage output.        |
| [`format-flag-arg`](cli-trooper.usage/format-flag-arg.html)     | Resolve the value label displayed for a flag specification. |
| [`format-flag-name`](cli-trooper.usage/format-flag-name.html)   | Format one or more flag spellings for usage output.         |
| [`format-positional`](cli-trooper.usage/format-positional.html) | Format a primitive positional declaration for usage output. |
| [`usage-string`](cli-trooper.usage/usage-string.html)           | Build application usage lines from canonical configuration. |

</main>
</div>
