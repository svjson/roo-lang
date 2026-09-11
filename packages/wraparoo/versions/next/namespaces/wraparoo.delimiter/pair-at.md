---
layout: reference
title: wraparoo.delimiter/pair-at
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.delimiter.html">wraparoo.delimiter</a>
<ol>
<li><a href="analyze.html">analyze</a></li>
<li><a class="active" href="pair-at.html">pair-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.delimiter.html">wraparoo.delimiter</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">pair-at</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.delimiter/pair-at.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.delimiter.html">wraparoo.delimiter</a>/</div>
<h1><code>pair-at</code></h1>
</header>

Return the delimiter pair touching scalar cursor POSITION in ANALYSIS.

<hr class="signature-divider">

```roo
(pair-at analysis position)
```

<hr class="signature-divider">

## Signature:

```roo
(pair-at analysis position)
```

| Arg      | Description                                          |
| -------- | ---------------------------------------------------- |
| analysis | Result returned by `wraparoo/analyze`.               |
| position | Unicode-scalar cursor offset between source scalars. |

### Returns:

The pair whose opening or closing range ends at POSITION, preferring the
delimiter immediately before the cursor. If none ends there, returns the pair
whose delimiter starts at POSITION. Returns `nil` when no pair touches the
cursor.

</main>
</div>
