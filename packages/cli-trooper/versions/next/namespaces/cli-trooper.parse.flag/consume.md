---
layout: reference
title: cli-trooper.parse.flag/consume
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<ol>
<li><a class="active" href="consume.html">consume</a></li>
<li><a href="consume-value.html">consume-value</a></li>
<li><a href="finalize.html">finalize</a></li>
<li><a href="flag-equals.html">flag=</a></li>
<li><a href="flag-p.html">flag?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">consume</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.flag/consume.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>/</div>
<h1><code>consume</code></h1>
</header>

Apply one flag-shaped argument to parser state.

<hr class="signature-divider">

```roo
(consume state command flags arg)
```

<hr class="signature-divider">

Long flags may carry an assigned value after `=`. A constant flag rejects an
assigned value. An optional-value boolean stores `true` when no recognized
explicit value follows it.

<hr class="signature-divider">

## Signature:

```roo
(consume state command flags arg)
```

| Arg     | Description                                                                                                |
| ------- | ---------------------------------------------------------------------------------------------------------- |
| state   | The current argument parser state, including the complete argument<br>sequence and current argument index. |
| command | The selected command literal or path.                                                                      |
| flags   | The applicable canonical flag map.                                                                         |
| arg     | The raw flag argument.                                                                                     |

### Returns:

Parser state with a constant value stored, a pending value context, an
assigned value consumed, or a structured option diagnostic.

</main>
</div>
