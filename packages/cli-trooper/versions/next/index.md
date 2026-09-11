---
layout: reference
title: cli-trooper next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper next</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper next

CLI argument parser and dispatcher

| Namespace                                                                                | Symbols |
| ---------------------------------------------------------------------------------------- | ------- |
| [cli-trooper](namespaces/cli-trooper.html)                                               | 4       |
| [cli-trooper.config](namespaces/cli-trooper.config.html)                                 | 2       |
| [cli-trooper.config.command](namespaces/cli-trooper.config.command.html)                 | 2       |
| [cli-trooper.config.feature](namespaces/cli-trooper.config.feature.html)                 | 1       |
| [cli-trooper.config.feature.help](namespaces/cli-trooper.config.feature.help.html)       | 2       |
| [cli-trooper.config.feature.version](namespaces/cli-trooper.config.feature.version.html) | 2       |
| [cli-trooper.config.flag](namespaces/cli-trooper.config.flag.html)                       | 4       |
| [cli-trooper.config.positional](namespaces/cli-trooper.config.positional.html)           | 1       |
| [cli-trooper.config.validate](namespaces/cli-trooper.config.validate.html)               | 6       |
| [cli-trooper.diagnostic](namespaces/cli-trooper.diagnostic.html)                         | 3       |
| [cli-trooper.feature.help](namespaces/cli-trooper.feature.help.html)                     | 2       |
| [cli-trooper.feature.version](namespaces/cli-trooper.feature.version.html)               | 2       |
| [cli-trooper.invoke](namespaces/cli-trooper.invoke.html)                                 | 3       |
| [cli-trooper.parse](namespaces/cli-trooper.parse.html)                                   | 1       |
| [cli-trooper.parse.argument](namespaces/cli-trooper.parse.argument.html)                 | 1       |
| [cli-trooper.parse.command](namespaces/cli-trooper.parse.command.html)                   | 3       |
| [cli-trooper.parse.default](namespaces/cli-trooper.parse.default.html)                   | 1       |
| [cli-trooper.parse.flag](namespaces/cli-trooper.parse.flag.html)                         | 5       |
| [cli-trooper.parse.positional](namespaces/cli-trooper.parse.positional.html)             | 3       |
| [cli-trooper.parse.validation](namespaces/cli-trooper.parse.validation.html)             | 1       |
| [cli-trooper.parse.value](namespaces/cli-trooper.parse.value.html)                       | 1       |
| [cli-trooper.usage](namespaces/cli-trooper.usage.html)                                   | 6       |
