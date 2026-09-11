---
layout: reference
title: voodoo.editor.text/default
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.text.html">voodoo.editor.text</a>
<ol>
<li><a class="active" href="default.html">default</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.editor.text.html">voodoo.editor.text</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">default</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor.text/default.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.text.html">voodoo.editor.text</a>/</div>
<h1><code>default</code></h1>
</header>

Default plain-text editor.



The editor inserts keystroke text and pasted text; supports scalar movement,
deletion, Home/End, Control-A/E, whitespace-delimited word movement with
Control-Left/Right, and deletion through the end with Control-K. Its value is
shaped as `{:layers [{:keymap {...} :on-text fn :on-paste fn}]}`.



</main>
</div>
