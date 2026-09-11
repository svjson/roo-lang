---
layout: reference
title: voodoo.session.input/dispatch-next!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.input.html">voodoo.session.input</a>
<ol>
<li><a class="active" href="dispatch-next-bang.html">dispatch-next!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.session.input.html">voodoo.session.input</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">dispatch-next!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session.input/dispatch-next-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.input.html">voodoo.session.input</a>/</div>
<h1><code>dispatch-next!</code></h1>
</header>

Read one portable terminal event from INPUT and dispatch it through
WIDGET-SESSION.

<hr class="signature-divider">

```roo
(dispatch-next! widget-session input)
```

<hr class="signature-divider">

The caller retains ownership of the surrounding input loop and can therefore
handle EOF, unconsumed keystrokes, resize, and widget actions without Voodoo
imposing application policy.

<hr class="signature-divider">

## Signature:

```roo
(dispatch-next! widget-session input)
```

| Arg            | Description                                              |
| -------------- | -------------------------------------------------------- |
| widget-session | Session whose focused widget receives the event.         |
| input          | Scoped terminal input supplied by `inpoots.stdin/with!`. |

### Returns:

A map containing the original portable `:event`, whether the focused widget
handled it, and the generic widget `:actions` returned by
`voodoo.session/dispatch!`:

```roo
{:event {:type :keystroke
:key :key/a
:text "a"}
:handled? true
:actions []}
```

</main>
</div>
