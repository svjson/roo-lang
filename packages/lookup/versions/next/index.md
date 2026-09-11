---
layout: reference
title: lookup next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup next</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup next

Roo symbol index generator.

| Namespace                                                                                      | Symbols |
| ---------------------------------------------------------------------------------------------- | ------- |
| [lookup.audit](namespaces/lookup.audit.html)                                                   | 10      |
| [lookup.cli](namespaces/lookup.cli.html)                                                       | 2       |
| [lookup.command.audit](namespaces/lookup.command.audit.html)                                   | 6       |
| [lookup.command.index](namespaces/lookup.command.index.html)                                   | 3       |
| [lookup.command.thing-at](namespaces/lookup.command.thing-at.html)                             | 4       |
| [lookup.doc](namespaces/lookup.doc.html)                                                       | 1       |
| [lookup.index](namespaces/lookup.index.html)                                                   | 7       |
| [lookup.index.incremental](namespaces/lookup.index.incremental.html)                           | 18      |
| [lookup.source](namespaces/lookup.source.html)                                                 | 16      |
| [lookup.source.docstring.parse.content](namespaces/lookup.source.docstring.parse.content.html) | 13      |
| [lookup.source.docstring.symbols](namespaces/lookup.source.docstring.symbols.html)             | 16      |
| [lookup.source.native.parse.comment](namespaces/lookup.source.native.parse.comment.html)       | 27      |
| [lookup.source.native.parse.entries](namespaces/lookup.source.native.parse.entries.html)       | 20      |
| [lookup.source.native.symbols](namespaces/lookup.source.native.symbols.html)                   | 14      |
| [lookup.source.path](namespaces/lookup.source.path.html)                                       | 4       |
