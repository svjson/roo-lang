---
layout: reference
title: proofread.rules.names.kebab-case-name
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.names.kebab-case-name.html">proofread.rules.names.kebab-case-name</a>
<ol>
<li><a href="proofread.rules.names.kebab-case-name/check.html">check</a></li>
<li><a href="proofread.rules.names.kebab-case-name/non-kebab-name-p.html">non-kebab-name?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.names.kebab-case-name</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.names.kebab-case-name.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.names.kebab-case-name

| Symbol                                                                           | Summary                                                   |
| -------------------------------------------------------------------------------- | --------------------------------------------------------- |
| [`check`](proofread.rules.names.kebab-case-name/check.html)                      | Return a `kebab-case-name` diagnostic for matching nodes. |
| [`non-kebab-name?`](proofread.rules.names.kebab-case-name/non-kebab-name-p.html) | Test whether an identifier should use kebab-case.         |

</main>
</div>
