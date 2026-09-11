---
layout: reference
title: cli-trooper.config.flag/normalize-spec
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>
<ol>
<li><a href="normalize-flags.html">normalize-flags</a></li>
<li><a href="normalize-name.html">normalize-name</a></li>
<li><a href="normalize-names.html">normalize-names</a></li>
<li><a class="active" href="normalize-spec.html">normalize-spec</a></li>
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
<span aria-current="page">normalize-spec</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.config.flag/normalize-spec.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.config.flag.html">cli-trooper.config.flag</a>/</div>
<h1><code>normalize-spec</code></h1>
</header>

Expand a flag specification to the current canonical flag shape.

<hr class="signature-divider">

```roo
(normalize-spec names spec)
```

<hr class="signature-divider">

## Signature:

```roo
(normalize-spec names spec)
```

| Arg   | Description                                             |
| ----- | ------------------------------------------------------- |
| names | The flag's normalized spellings.                        |
| spec  | The supplied flag specification or supported shorthand. |

### Returns:

A canonical value-producing flag specification with explicit `:type`,
`:required?`, `:repeatable?`, `:default`, and `:destination` entries. Constant
flags also carry `:value`; boolean flags carry `:optional-value?`. A
command-shaped flag has no value storage fields. An explicit default may be a
literal value or a resolver function accepting the available options map.

</main>
</div>
