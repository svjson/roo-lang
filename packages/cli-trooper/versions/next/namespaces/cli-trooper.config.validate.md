---
layout: reference
title: cli-trooper.config.validate
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.config.validate.html">cli-trooper.config.validate</a>
<ol>
<li><a href="cli-trooper.config.validate/flag-errors.html">flag-errors</a></li>
<li><a href="cli-trooper.config.validate/input-errors.html">input-errors</a></li>
<li><a href="cli-trooper.config.validate/input-flag-errors.html">input-flag-errors</a></li>
<li><a href="cli-trooper.config.validate/missing-mixin-errors.html">missing-mixin-errors</a></li>
<li><a href="cli-trooper.config.validate/model-errors.html">model-errors</a></li>
<li><a href="cli-trooper.config.validate/validate-flag-entry.html">validate-flag-entry</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.config.validate</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.config.validate.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.config.validate

| Symbol                                                                          | Summary                                                                 |
| ------------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| [`flag-errors`](cli-trooper.config.validate/flag-errors.html)                   | Find spelling collisions in a canonical flag map.                       |
| [`input-errors`](cli-trooper.config.validate/input-errors.html)                 | Validate concerns that must be detected before shorthand is discarded.  |
| [`input-flag-errors`](cli-trooper.config.validate/input-flag-errors.html)       | Find flag collisions that would be hidden by canonical map association. |
| [`missing-mixin-errors`](cli-trooper.config.validate/missing-mixin-errors.html) | Find commands that reference undeclared mixins.                         |
| [`model-errors`](cli-trooper.config.validate/model-errors.html)                 | Validate flag-name ownership in a normalized configuration model.       |
| [`validate-flag-entry`](cli-trooper.config.validate/validate-flag-entry.html)   | Validate every spelling in one canonical flag entry.                    |

</main>
</div>
