---
layout: reference
title: proof next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof next</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof next

Roo test framework.

| Namespace                                                            | Symbols |
| -------------------------------------------------------------------- | ------- |
| [proof.assertion](namespaces/proof.assertion.html)                   | 3       |
| [proof.cli](namespaces/proof.cli.html)                               | 2       |
| [proof.core](namespaces/proof.core.html)                             | 18      |
| [proof.fixture](namespaces/proof.fixture.html)                       | 12      |
| [proof.fixture.cache](namespaces/proof.fixture.cache.html)           | 3       |
| [proof.fixture.persistent](namespaces/proof.fixture.persistent.html) | 6       |
| [proof.fixture.value](namespaces/proof.fixture.value.html)           | 1       |
| [proof.reporter](namespaces/proof.reporter.html)                     | 5       |
| [proof.reporter.common](namespaces/proof.reporter.common.html)       | 7       |
| [proof.reporter.simple](namespaces/proof.reporter.simple.html)       | 5       |
| [proof.reporter.tree](namespaces/proof.reporter.tree.html)           | 8       |
| [proof.runner](namespaces/proof.runner.html)                         | 13      |
| [proof.scenario](namespaces/proof.scenario.html)                     | 3       |
| [proof.suite](namespaces/proof.suite.html)                           | 9       |
| [proof.tool](namespaces/proof.tool.html)                             | 1       |
