---
layout: reference
title: voodoo.editor/dispatch
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.html">voodoo.editor</a>
<ol>
<li><a href="compose.html">compose</a></li>
<li><a class="active" href="dispatch.html">dispatch</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.editor.html">voodoo.editor</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">dispatch</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor/dispatch.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.html">voodoo.editor</a>/</div>
<h1><code>dispatch</code></h1>
</header>

Dispatch EVENT through EDITOR against BUFFER.

<hr class="signature-divider">

```roo
(dispatch editor buffer event)
```

<hr class="signature-divider">

Each editor layer may contain a `:keymap`, a `:textmap`, an `:on-text`
function, and an `:on-paste` function. Presentation layers may additionally
contain `:decorations` and `:styling` functions consumed by
`voodoo.editor.presentation`. Key and exact-text functions receive BUFFER.
Text and paste functions receive BUFFER and the event text. A function
returns the next buffer when it handles the event or `nil` to let a lower
layer try it.

<hr class="signature-divider">

## Signature:

```roo
(dispatch editor buffer event)
```

| Arg    | Description                               |
| ------ | ----------------------------------------- |
| editor | Editor map containing a `:layers` vector. |
| buffer | Current editable text buffer.             |
| event  | Portable input event to dispatch.         |

### Returns:

The first buffer returned by a handling layer, or `nil` when every layer
declines the event.

</main>
</div>
