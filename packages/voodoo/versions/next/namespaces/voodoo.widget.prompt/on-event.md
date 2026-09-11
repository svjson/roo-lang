---
layout: reference
title: voodoo.widget.prompt/on-event
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.prompt.html">voodoo.widget.prompt</a>
<ol>
<li><a href="make.html">make</a></li>
<li><a class="active" href="on-event.html">on-event</a></li>
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
<span aria-current="page">on-event</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget.prompt/on-event.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.prompt.html">voodoo.widget.prompt</a>/</div>
<h1><code>on-event</code></h1>
</header>

Apply a portable terminal EVENT to PROMPT.

<hr class="signature-divider">

```roo
(on-event prompt event)
```

<hr class="signature-divider">

## Signature:

```roo
(on-event prompt event)
```

| Arg    | Description                       |
| ------ | --------------------------------- |
| prompt | Prompt widget returned by `make`. |
| event  | Portable terminal event to apply. |

### Returns:

A widget event result. Editing and history navigation events return
`{:widget prompt}`. Enter returns a generic submit action containing the
current value:

```roo
{:actions
[{:type :submit
:value "(+ 1 2)"}]}
```

Control-C clears a nonempty current edit and returns a generic cancel action.
Control-C on an empty edit remains unhandled. Other unrecognized keystrokes
and events also return an empty result so another interaction layer may give
them meaning later.

</main>
</div>
