---
layout: reference
title: voodoo.session/add!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.html">voodoo.session</a>
<ol>
<li><a class="active" href="add-bang.html">add!</a></li>
<li><a href="clear-rows-upward.html">clear-rows-upward</a></li>
<li><a href="cursor-baseline-distance.html">cursor-baseline-distance</a></li>
<li><a href="dispatch-bang.html">dispatch!</a></li>
<li><a href="evict-bang.html">evict!</a></li>
<li><a href="focus-bang.html">focus!</a></li>
<li><a href="make.html">make</a></li>
<li><a href="print-lines-bang.html">print-lines!</a></li>
<li><a href="redraw-bang.html">redraw!</a></li>
<li><a href="remove-bang.html">remove!</a></li>
<li><a href="reset-terminal-cursor-bang.html">reset-terminal-cursor!</a></li>
<li><a href="restore-terminal-cursor-bang.html">restore-terminal-cursor!</a></li>
<li><a href="total-height.html">total-height</a></li>
<li><a href="update-bang.html">update!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.session.html">voodoo.session</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">add!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/add-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>add!</code></h1>
</header>

Register a new widget in a session and render it immediately.

<hr class="signature-divider">

```roo
(add! {:keys [id-seq widgets order tty?] :as session} spec)
```

<hr class="signature-divider">

## Signature:

```roo
(add! {:keys [id-seq widgets order tty?] :as session} spec)
```

| Arg                                             | Description                                      |
| ----------------------------------------------- | ------------------------------------------------ |
| {:keys [id-seq widgets order tty?] :as session} |                                                  |
| spec                                            | Widget spec map. Built via `voodoo.widget/make`. |

### Returns:

A handle to pass into `update!`/`remove!`.

In tty mode, the widget is always shown and tracked, regardless of
`spec`'s `:degrade` policy - a live terminal is exactly what the policy
degrades *for*. In append mode, whether the widget is registered at all
is read off `:on-add` first: `:untracked` means it never enters
`:widgets`/`:order`, so later `update!`/`remove!` calls against its
handle are no-ops - the caller never has to know whether it's running
in a tty to make that true. Otherwise the widget is registered, and
`:on-add` (`:visible`/`:hidden`) decides whether its text is printed.
See `voodoo.widget/make`.

- `session`: Session returned by `make`.

### Returns:

A handle to pass into `update!`/`remove!`.

In tty mode, the widget is always shown and tracked, regardless of
`spec`'s `:degrade` policy - a live terminal is exactly what the policy
degrades *for*. In append mode, whether the widget is registered at all
is read off `:on-add` first: `:untracked` means it never enters
`:widgets`/`:order`, so later `update!`/`remove!` calls against its
handle are no-ops - the caller never has to know whether it's running
in a tty to make that true. Otherwise the widget is registered, and
`:on-add` (`:visible`/`:hidden`) decides whether its text is printed.
See `voodoo.widget/make`.

</main>
</div>
