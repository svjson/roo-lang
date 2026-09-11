---
layout: reference
title: cli-trooper.config.flag
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.config.flag.html">cli-trooper.config.flag</a>
<ol>
<li><a href="cli-trooper.config.flag/normalize-flags.html">normalize-flags</a></li>
<li><a href="cli-trooper.config.flag/normalize-name.html">normalize-name</a></li>
<li><a href="cli-trooper.config.flag/normalize-names.html">normalize-names</a></li>
<li><a href="cli-trooper.config.flag/normalize-spec.html">normalize-spec</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.config.flag</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.config.flag.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.config.flag

| Symbol                                                            | Summary                                                          |
| ----------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`normalize-flags`](cli-trooper.config.flag/normalize-flags.html) | Normalize a map of ergonomic flag declarations.                  |
| [`normalize-name`](cli-trooper.config.flag/normalize-name.html)   | Normalize one flag spelling.                                     |
| [`normalize-names`](cli-trooper.config.flag/normalize-names.html) | Normalize all spellings of a flag into an explicit vector.       |
| [`normalize-spec`](cli-trooper.config.flag/normalize-spec.html)   | Expand a flag specification to the current canonical flag shape. |

</main>
</div>
