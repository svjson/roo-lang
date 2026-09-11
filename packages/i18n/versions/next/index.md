---
layout: reference
title: i18n next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">i18n next</span>
</nav>
<div class="version-context">
<span class="version-package">i18n</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["i18n"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# i18n next

Translation catalog library for Roo programs.

| Namespace                    | Symbols |
| ---------------------------- | ------- |
| [i18n](namespaces/i18n.html) | 9       |
