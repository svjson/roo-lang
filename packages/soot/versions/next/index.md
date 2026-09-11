---
layout: reference
title: soot next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">soot next</span>
</nav>
<div class="version-context">
<span class="version-package">soot</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["soot"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# soot next

Small ANSI SGR styling functions for Roo strings.

| Namespace                    | Symbols |
| ---------------------------- | ------- |
| [soot](namespaces/soot.html) | 42      |
