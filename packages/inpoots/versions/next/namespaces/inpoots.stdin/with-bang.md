---
layout: reference
title: inpoots.stdin/with!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../inpoots.stdin.html">inpoots.stdin</a>
<ol>
<li><a href="read-event-bang.html">read-event!</a></li>
<li><a class="active" href="with-bang.html">with!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">inpoots next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../inpoots.stdin.html">inpoots.stdin</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">with!</span>
</nav>
<div class="version-context">
<span class="version-package">inpoots</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["inpoots"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/inpoots.stdin/with-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../inpoots.stdin.html">inpoots.stdin</a>/</div>
<h1><code>with!</code></h1>
</header>

Run CALLBACK with scoped access to standard input.



OPTIONS accepts `:mode :cooked`, `:mode :cbreak`, or `:mode :raw`; raw is
the default. The callback receives an input value accepted by `read-event!`.
The original input mode is restored and the input becomes inactive when the
callback exits.

Returns the callback's result.



</main>
</div>
