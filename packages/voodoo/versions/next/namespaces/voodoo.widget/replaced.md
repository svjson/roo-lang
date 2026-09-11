---
layout: reference
title: voodoo.widget/replaced
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.html">voodoo.widget</a>
<ol>
<li><a href="count-char.html">count-char</a></li>
<li><a href="line-count.html">line-count</a></li>
<li><a href="make.html">make</a></li>
<li><a class="active" href="replaced.html">replaced</a></li>
<li><a href="silent.html">silent</a></li>
<li><a href="tty-only.html">tty-only</a></li>
<li><a href="visible-once.html">visible-once</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.widget.html">voodoo.widget</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">replaced</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget/replaced.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.html">voodoo.widget</a>/</div>
<h1><code>replaced</code></h1>
</header>

Append-mode degrade policy for a widget whose `add!`-time text is a
placeholder not worth logging (e.g. "RUNNING x"), but whose first
`update!` carries its real, final content (e.g. "PASS x") - the one
line append mode should see. For a widget that starts in-progress and
is updated exactly once, to its finished state.





</main>
</div>
