---
layout: reference
title: voodoo next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo next</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo next

Terminal layout, composable editing, and interactive widgets for Roo.

| Namespace                                                                | Symbols |
| ------------------------------------------------------------------------ | ------- |
| [voodoo.ansi](namespaces/voodoo.ansi.html)                               | 4       |
| [voodoo.editor](namespaces/voodoo.editor.html)                           | 2       |
| [voodoo.editor.presentation](namespaces/voodoo.editor.presentation.html) | 5       |
| [voodoo.editor.text](namespaces/voodoo.editor.text.html)                 | 1       |
| [voodoo.session](namespaces/voodoo.session.html)                         | 14      |
| [voodoo.session.input](namespaces/voodoo.session.input.html)             | 1       |
| [voodoo.text.buffer](namespaces/voodoo.text.buffer.html)                 | 13      |
| [voodoo.widget](namespaces/voodoo.widget.html)                           | 7       |
| [voodoo.widget.prompt](namespaces/voodoo.widget.prompt.html)             | 2       |
| [voodoo.widget.spinner](namespaces/voodoo.widget.spinner.html)           | 3       |
