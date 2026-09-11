---
layout: reference
title: cli-trooper.feature.help/lines
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.feature.help.html">cli-trooper.feature.help</a>
<ol>
<li><a class="active" href="lines.html">lines</a></li>
<li><a href="run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.feature.help.html">cli-trooper.feature.help</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lines</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.feature.help/lines.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.feature.help.html">cli-trooper.feature.help</a>/</div>
<h1><code>lines</code></h1>
</header>

Build help output for one command level.

<hr class="signature-divider">

```roo
(lines config command-path)
```

<hr class="signature-divider">

## Signature:

```roo
(lines config command-path)
```

| Arg          | Description                                                                                                                                                                                   |
| ------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| config       | A canonical cli-trooper configuration model.                                                                                                                                                  |
| command-path | The ordinary command path whose attached help declaration<br>was selected. Use an empty sequence for application-level help. The current<br>parser supports at most one command in this path. |

### Returns:

A sequence of help lines from the selected level's configured `:before`,
generated or custom `:output`, and `:after` sections. Returns an empty
sequence when help is not attached at that level.

</main>
</div>
