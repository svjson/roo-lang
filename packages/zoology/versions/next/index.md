---
layout: reference
title: zoology next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">zoology next</span>
</nav>
<div class="version-context">
<span class="version-package">zoology</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["zoology"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# zoology next

Generic schema resolution, canonicalization, and validation for Roo data.

| Namespace                                                    | Symbols |
| ------------------------------------------------------------ | ------- |
| [zoology.canonical](namespaces/zoology.canonical.html)       | 11      |
| [zoology.schema](namespaces/zoology.schema.html)             | 34      |
| [zoology.valid-values](namespaces/zoology.valid-values.html) | 4       |
| [zoology.validation](namespaces/zoology.validation.html)     | 14      |
