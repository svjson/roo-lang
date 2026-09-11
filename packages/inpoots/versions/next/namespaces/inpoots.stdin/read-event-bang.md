---
layout: reference
title: inpoots.stdin/read-event!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../inpoots.stdin.html">inpoots.stdin</a>
<ol>
<li><a class="active" href="read-event-bang.html">read-event!</a></li>
<li><a href="with-bang.html">with!</a></li>
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
<span aria-current="page">read-event!</span>
</nav>
<div class="version-context">
<span class="version-package">inpoots</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["inpoots"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/inpoots.stdin/read-event-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../inpoots.stdin.html">inpoots.stdin</a>/</div>
<h1><code>read-event!</code></h1>
</header>

Block until INPUT yields a portable terminal event.



INPUT is the scoped value supplied to a `with!` callback. Returns a
keystroke, paste, resize, or stream-EOF event map. Keystrokes contain a
normalized `:key` lookup value when Inpoots can infer one, and contain
`:text` when the terminal produced text. Terminal control keystrokes may
contain `:signal` or `:control` metadata for an unhandled interaction layer
to apply.



</main>
</div>
