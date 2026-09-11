---
layout: reference
title: voodoo.widget/tty-only
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.html">voodoo.widget</a>
<ol>
<li><a href="count-char.html">count-char</a></li>
<li><a href="line-count.html">line-count</a></li>
<li><a href="make.html">make</a></li>
<li><a href="replaced.html">replaced</a></li>
<li><a href="silent.html">silent</a></li>
<li><a class="active" href="tty-only.html">tty-only</a></li>
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
<span aria-current="page">tty-only</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget/tty-only.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.html">voodoo.widget</a>/</div>
<h1><code>tty-only</code></h1>
</header>

Append-mode degrade policy for a widget that doesn't exist outside a
live tty at all: not shown, and not even registered in the session, so
`update!`/`remove!` calls against its handle are no-ops in append mode
- there's nothing there to find, not a policy decision either of them
has to consult. For an in-progress indicator (e.g. "RUNNING x") whose
caller prints its own separate, final line once the widget is removed
- the in-progress line itself has no place in a static log, and the
caller never has to ask whether it's running in a tty to make that
true.





</main>
</div>
