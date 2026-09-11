---
layout: reference
title: cli-trooper.parse.positional
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.parse.positional.html">cli-trooper.parse.positional</a>
<ol>
<li><a href="cli-trooper.parse.positional/consume.html">consume</a></li>
<li><a href="cli-trooper.parse.positional/finalize.html">finalize</a></li>
<li><a href="cli-trooper.parse.positional/positional-p.html">positional?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.parse.positional</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.parse.positional.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.parse.positional

| Symbol                                                          | Summary                                                                    |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- |
| [`consume`](cli-trooper.parse.positional/consume.html)          | Capture one raw argument for the current primitive positional declaration. |
| [`finalize`](cli-trooper.parse.positional/finalize.html)        | Record required primitive positionals not reached before input ended.      |
| [`positional?`](cli-trooper.parse.positional/positional-p.html) | Determine whether an argument has the prototype's positional token shape.  |

</main>
</div>
