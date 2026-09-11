---
layout: reference
title: cli-trooper.config.flag/normalize-flags
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>
<ol>
<li><a class="active" href="normalize-flags.html">normalize-flags</a></li>
<li><a href="normalize-name.html">normalize-name</a></li>
<li><a href="normalize-names.html">normalize-names</a></li>
<li><a href="normalize-spec.html">normalize-spec</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">normalize-flags</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.flag/normalize-flags.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>/</div>
<h1><code>normalize-flags</code></h1>
</header>

Normalize a map of ergonomic flag declarations.

<hr class="signature-divider">

```roo
(normalize-flags flags)
```

<hr class="signature-divider">

## Signature:

```roo
(normalize-flags flags)
```

| Arg   | Description                                               |
| ----- | --------------------------------------------------------- |
| flags | A supplied flag map, or `nil` when no flags are declared. |

### Returns:

A canonical flag map keyed by vectors of normalized spellings.

</main>
</div>
