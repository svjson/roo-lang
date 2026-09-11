---
layout: reference
title: voodoo.widget.prompt/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.prompt.html">voodoo.widget.prompt</a>
<ol>
<li><a class="active" href="make.html">make</a></li>
<li><a href="on-event.html">on-event</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.widget.prompt.html">voodoo.widget.prompt</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget.prompt/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.prompt.html">voodoo.widget.prompt</a>/</div>
<h1><code>make</code></h1>
</header>

Create a focusable prompt widget.

<hr class="signature-divider">

```roo
(make opts)
```

<hr class="signature-divider">

## Signature:

```roo
(make opts)
```

| Arg  | Description                                                                                                                                                                                                                                                                                                                     |
| ---- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| opts | Widget option map. `:prompt` sets the displayed prefix, `:value`<br>sets the initial editable text, and `:history` supplies earlier values for<br>Up and Down navigation. `:editor` supplies a `voodoo.editor` definition;<br>when omitted, the prompt uses `voodoo.editor.text/default`. Other widget<br>fields are preserved. |

### Returns:

A focusable prompt widget. Its history ends with the current editable value,
and its history position starts at that final entry. The prompt defaults to
`voodoo.widget/tty-only` degradation. Its rendered `:text` and cursor prefix
derive from the editor's composable decorations, styling, and optional
top-level `:paint` function; the buffer and submitted value remain plain.

```roo
(prompt/make {:prompt "roo> "
:value "(+ 1 2)"})
```

</main>
</div>
