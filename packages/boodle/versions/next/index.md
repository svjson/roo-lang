---
layout: reference
title: boodle next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle next</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle next

Reference documentation catalog tooling for Roo.

| Namespace                                                                | Symbols |
| ------------------------------------------------------------------------ | ------- |
| [boodle.cli](namespaces/boodle.cli.html)                                 | 3       |
| [boodle.command.generate](namespaces/boodle.command.generate.html)       | 18      |
| [boodle.format.github-pages](namespaces/boodle.format.github-pages.html) | 52      |
| [boodle.format.lookup](namespaces/boodle.format.lookup.html)             | 10      |
| [boodle.model](namespaces/boodle.model.html)                             | 5       |
| [boodle.output.files](namespaces/boodle.output.files.html)               | 1       |
| [boodle.publication](namespaces/boodle.publication.html)                 | 16      |
| [boodle.topology.reference](namespaces/boodle.topology.reference.html)   | 21      |
