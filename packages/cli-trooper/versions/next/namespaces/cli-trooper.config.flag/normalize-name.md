---
layout: reference
title: cli-trooper.config.flag/normalize-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>
<ol>
<li><a href="normalize-flags.html">normalize-flags</a></li>
<li><a class="active" href="normalize-name.html">normalize-name</a></li>
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
<span aria-current="page">normalize-name</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.flag/normalize-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>/</div>
<h1><code>normalize-name</code></h1>
</header>

Normalize one flag spelling.

<hr class="signature-divider">

```roo
(normalize-name value)
```

<hr class="signature-divider">

Single-character names receive one dash and longer names receive two. An
already dashed name is retained.

<hr class="signature-divider">

## Signature:

```roo
(normalize-name value)
```

| Arg   | Description                 |
| ----- | --------------------------- |
| value | The supplied flag spelling. |

### Returns:

The normalized flag spelling, `nil` for an empty string, or a non-string input
unchanged.

</main>
</div>
