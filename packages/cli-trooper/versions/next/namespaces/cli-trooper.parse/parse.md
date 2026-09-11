---
layout: reference
title: cli-trooper.parse/parse
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.html">cli-trooper.parse</a>
<ol>
<li><a class="active" href="parse.html">parse</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.html">cli-trooper.parse</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">parse</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse/parse.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.html">cli-trooper.parse</a>/</div>
<h1><code>parse</code></h1>
</header>

Parse an argument sequence against a normalized cli-trooper configuration.

<hr class="signature-divider">

```roo
(parse config args & supplied-options &rest)
```

<hr class="signature-divider">

## Signature:

```roo
(parse config args & supplied-options &rest)
```

| Arg              | Description                                                                                                  |
| ---------------- | ------------------------------------------------------------------------------------------------------------ |
| config           | A normalized cli-trooper configuration model.                                                                |
| args             | The CLI argument sequence to parse.                                                                          |
| supplied-options | Optional canonical option defaults supplied by the<br>invocation. Explicit arguments override these options. |
| &rest            |                                                                                                              |

### Returns:

A map containing the selected `:command` identity, parsed `:options`, and
structured `:errors`. The command is `nil` and options are empty when command
selection fails.

</main>
</div>
