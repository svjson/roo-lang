---
layout: reference
title: workbook next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">workbook next</span>
</nav>
<div class="version-context">
<span class="version-package">workbook</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["workbook"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# workbook next

Controlled state updates with patch history for Roo programs.

| Namespace                                                      | Symbols |
| -------------------------------------------------------------- | ------- |
| [workbook](namespaces/workbook.html)                           | 18      |
| [workbook.core](namespaces/workbook.core.html)                 | 19      |
| [workbook.history](namespaces/workbook.history.html)           | 12      |
| [workbook.patch.apply](namespaces/workbook.patch.apply.html)   | 8       |
| [workbook.patch.intent](namespaces/workbook.patch.intent.html) | 5       |
| [workbook.value](namespaces/workbook.value.html)               | 11      |
