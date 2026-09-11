---
layout: reference
title: roopl next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roopl next</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roopl next

Interactive terminal shell for Roo.

| Namespace                                                                    | Symbols |
| ---------------------------------------------------------------------------- | ------- |
| [roopl.editor](namespaces/roopl.editor.html)                                 | 4       |
| [roopl.history](namespaces/roopl.history.html)                               | 4       |
| [roopl.repl](namespaces/roopl.repl.html)                                     | 4       |
| [roopl.target](namespaces/roopl.target.html)                                 | 2       |
| [roopl.target.local.dedicated](namespaces/roopl.target.local.dedicated.html) | 1       |
| [roopl.terminal](namespaces/roopl.terminal.html)                             | 1       |
