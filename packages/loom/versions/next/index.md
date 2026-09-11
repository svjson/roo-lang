---
layout: reference
title: loom next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">loom next</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# loom next

Roo package manager.

| Namespace                                                                    | Symbols |
| ---------------------------------------------------------------------------- | ------- |
| [loom.command.bootstrap](namespaces/loom.command.bootstrap.html)             | 4       |
| [loom.command.deps](namespaces/loom.command.deps.html)                       | 10      |
| [loom.command.info](namespaces/loom.command.info.html)                       | 1       |
| [loom.command.init](namespaces/loom.command.init.html)                       | 4       |
| [loom.command.install](namespaces/loom.command.install.html)                 | 5       |
| [loom.command.link](namespaces/loom.command.link.html)                       | 1       |
| [loom.command.list](namespaces/loom.command.list.html)                       | 5       |
| [loom.command.uninstall](namespaces/loom.command.uninstall.html)             | 1       |
| [loom.common.args](namespaces/loom.common.args.html)                         | 2       |
| [loom.common.dependency-graph](namespaces/loom.common.dependency-graph.html) | 25      |
| [loom.common.files](namespaces/loom.common.files.html)                       | 2       |
| [loom.common.manifest](namespaces/loom.common.manifest.html)                 | 15      |
| [loom.common.package-spec](namespaces/loom.common.package-spec.html)         | 2       |
| [loom.common.repository](namespaces/loom.common.repository.html)             | 4       |
| [loom.core](namespaces/loom.core.html)                                       | 13      |
| [loom.package.preflight](namespaces/loom.package.preflight.html)             | 7       |
