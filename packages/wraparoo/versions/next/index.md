---
layout: reference
title: wraparoo next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wraparoo next</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# wraparoo next

Frontend-neutral analysis and structural editing of Roo source.

| Namespace                                                | Symbols |
| -------------------------------------------------------- | ------- |
| [wraparoo](namespaces/wraparoo.html)                     | 1       |
| [wraparoo.delimiter](namespaces/wraparoo.delimiter.html) | 2       |
| [wraparoo.lex](namespaces/wraparoo.lex.html)             | 8       |
| [wraparoo.syntax](namespaces/wraparoo.syntax.html)       | 6       |
