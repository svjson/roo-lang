---
layout: reference
title: lookup.source.path
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.source.path.html">lookup.source.path</a>
<ol>
<li><a href="lookup.source.path/canonical.html">canonical</a></li>
<li><a href="lookup.source.path/excluded-p.html">excluded?</a></li>
<li><a href="lookup.source.path/package-relative.html">package-relative</a></li>
<li><a href="lookup.source.path/within-p.html">within?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.source.path</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.source.path.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.source.path

| Symbol                                                         | Summary                                                     |
| -------------------------------------------------------------- | ----------------------------------------------------------- |
| [`canonical`](lookup.source.path/canonical.html)               | Return PATH as a normalized absolute path.                  |
| [`excluded?`](lookup.source.path/excluded-p.html)              | Test whether PATH equals or descends from an excluded path. |
| [`package-relative`](lookup.source.path/package-relative.html) | Return PATH relative to PACKAGE-DIR.                        |
| [`within?`](lookup.source.path/within-p.html)                  | Test whether PATH equals or descends from ROOT.             |

</main>
</div>
