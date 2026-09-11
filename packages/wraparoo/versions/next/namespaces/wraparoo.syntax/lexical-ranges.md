---
layout: reference
title: wraparoo.syntax/lexical-ranges
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../wraparoo.syntax.html">wraparoo.syntax</a>
<ol>
<li><a href="begin-element.html">begin-element</a></li>
<li><a href="classify.html">classify</a></li>
<li><a href="constants.html">constants</a></li>
<li><a class="active" href="lexical-ranges.html">lexical-ranges</a></li>
<li><a href="local-name-range.html">local-name-range</a></li>
<li><a href="special-forms.html">special-forms</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../wraparoo.syntax.html">wraparoo.syntax</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lexical-ranges</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/wraparoo.syntax/lexical-ranges.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../wraparoo.syntax.html">wraparoo.syntax</a>/</div>
<h1><code>lexical-ranges</code></h1>
</header>

Classify TOKEN using lexical facts independent of its containing form.

<hr class="signature-divider">

```roo
(lexical-ranges {:keys [kind text range]})
```

<hr class="signature-divider">

## Signature:

```roo
(lexical-ranges {:keys [kind text range]})
```

| Arg                       | Description |
| ------------------------- | ----------- |
| {:keys [kind text range]} |             |

### Returns:

A vector of typed ranges. Qualified keywords are split into keyword marker,
qualifier, and local-name ranges. Qualified symbols contribute a qualifier
range while their local name remains available for contextual classification.

- `token`: One token returned by `wraparoo.lex/scan`.

### Returns:

A vector of typed ranges. Qualified keywords are split into keyword marker,
qualifier, and local-name ranges. Qualified symbols contribute a qualifier
range while their local name remains available for contextual classification.

</main>
</div>
