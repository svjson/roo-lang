---
layout: reference
title: voodoo.session.input
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.session.input.html">voodoo.session.input</a>
<ol>
<li><a href="voodoo.session.input/dispatch-next-bang.html">dispatch-next!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.session.input</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.session.input.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.session.input

| Symbol                                                           | Summary                                                                                |
| ---------------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| [`dispatch-next!`](voodoo.session.input/dispatch-next-bang.html) | Read one portable terminal event from INPUT and dispatch it through<br>WIDGET-SESSION. |

</main>
</div>
