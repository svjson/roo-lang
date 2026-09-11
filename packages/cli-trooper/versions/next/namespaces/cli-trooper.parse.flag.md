---
layout: reference
title: cli-trooper.parse.flag
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="cli-trooper.parse.flag.html">cli-trooper.parse.flag</a>
<ol>
<li><a href="cli-trooper.parse.flag/consume.html">consume</a></li>
<li><a href="cli-trooper.parse.flag/consume-value.html">consume-value</a></li>
<li><a href="cli-trooper.parse.flag/finalize.html">finalize</a></li>
<li><a href="cli-trooper.parse.flag/flag-equals.html">flag=</a></li>
<li><a href="cli-trooper.parse.flag/flag-p.html">flag?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">cli-trooper.parse.flag</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/cli-trooper.parse.flag.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# cli-trooper.parse.flag

| Symbol                                                       | Summary                                                     |
| ------------------------------------------------------------ | ----------------------------------------------------------- |
| [`consume`](cli-trooper.parse.flag/consume.html)             | Apply one flag-shaped argument to parser state.             |
| [`consume-value`](cli-trooper.parse.flag/consume-value.html) | Capture a raw argument for the pending value-taking flag.   |
| [`finalize`](cli-trooper.parse.flag/finalize.html)           | Record unfinished and absent required flag values.          |
| [`flag=`](cli-trooper.parse.flag/flag-equals.html)           | Determine whether an argument matches a canonical flag key. |
| [`flag?`](cli-trooper.parse.flag/flag-p.html)                | Determine if an argument is a flag-shaped string.           |

</main>
</div>
