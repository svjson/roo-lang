---
layout: reference
title: spool next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">spool next</span>
</nav>
<div class="version-context">
<span class="version-package">spool</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["spool"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# spool next

Frame-scoped context timing reports for Roo programs.

| Namespace                                      | Symbols |
| ---------------------------------------------- | ------- |
| [spool](namespaces/spool.html)                 | 6       |
| [spool.context](namespaces/spool.context.html) | 7       |
| [spool.report](namespaces/spool.report.html)   | 4       |
