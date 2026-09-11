---
layout: reference
title: inpoots next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">inpoots next</span>
</nav>
<div class="version-context">
<span class="version-package">inpoots</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["inpoots"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# inpoots next

Terminal input and event handling for Roo.

| Namespace                                      | Symbols |
| ---------------------------------------------- | ------- |
| [inpoots.stdin](namespaces/inpoots.stdin.html) | 2       |
