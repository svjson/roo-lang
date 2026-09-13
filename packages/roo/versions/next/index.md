---
layout: reference
title: roo next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roo next</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roo next

| Namespace                                | Symbols |
| ---------------------------------------- | ------- |
| [roo](namespaces/roo.html)               | 189     |
| [roo.ast](namespaces/roo.ast.html)       | 2       |
| [roo.io](namespaces/roo.io.html)         | 24      |
| [roo.string](namespaces/roo.string.html) | 4       |
| [roo.tty](namespaces/roo.tty.html)       | 2       |
| [roo.worker](namespaces/roo.worker.html) | 5       |
