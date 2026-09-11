---
layout: reference
title: cli-trooper.parse.flag/consume-value
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<ol>
<li><a href="consume.html">consume</a></li>
<li><a class="active" href="consume-value.html">consume-value</a></li>
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
<span aria-current="page">consume-value</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.flag/consume-value.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>/</div>
<h1><code>consume-value</code></h1>
</header>

Capture a raw argument for the pending value-taking flag.

<hr class="signature-divider">

```roo
(consume-value state command arg)
```

<hr class="signature-divider">

## Signature:

```roo
(consume-value state command arg)
```

| Arg     | Description                                          |
| ------- | ---------------------------------------------------- |
| state   | Parser state containing a pending flag under `:ctx`. |
| command | The selected command literal.                        |
| arg     | The raw argument value to capture.                   |

### Returns:

Parser state with the converted value or conversion diagnostic recorded and
the pending context cleared.

</main>
</div>
