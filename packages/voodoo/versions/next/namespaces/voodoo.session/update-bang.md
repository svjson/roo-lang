---
layout: reference
title: voodoo.session/update!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.html">voodoo.session</a>
<ol>
<li><a href="add-bang.html">add!</a></li>
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
<li><a class="active" href="update-bang.html">update!</a></li>
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
<span aria-current="page">update!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/update-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>update!</code></h1>
</header>

Replace a widget's spec and re-render it in place.

<hr class="signature-divider">

```roo
(update! {:keys [order widgets tty?] :as session} handle spec)
```

<hr class="signature-divider">

## Signature:

```roo
(update! {:keys [order widgets tty?] :as session} handle spec)
```

| Arg                                      | Description                                                   |
| ---------------------------------------- | ------------------------------------------------------------- |
| {:keys [order widgets tty?] :as session} |                                                               |
| handle                                   | Handle returned by `add!`.                                    |
| spec                                     | New widget spec, replacing the widget's current one entirely. |

### Returns:

`handle`. A no-op (beyond returning `handle`) if `handle` isn't
currently tracked by `session` - e.g. a widget added with an
append-mode `:on-add :untracked` policy while not in a tty.

In tty mode, the widget (and anything below it) is erased and redrawn
in place - accounting for the widget's height changing (e.g. a test
going from a one-line "RUNNING" to a multi-line failure result), by
measuring how far to move up *before* replacing its text. In append
mode, whether the new text is printed is read straight off `spec`'s
`:degrade` policy (`:on-update`) - see `voodoo.widget/make`.

- `session`: Session returned by `make`.

### Returns:

`handle`. A no-op (beyond returning `handle`) if `handle` isn't
currently tracked by `session` - e.g. a widget added with an
append-mode `:on-add :untracked` policy while not in a tty.

In tty mode, the widget (and anything below it) is erased and redrawn
in place - accounting for the widget's height changing (e.g. a test
going from a one-line "RUNNING" to a multi-line failure result), by
measuring how far to move up *before* replacing its text. In append
mode, whether the new text is printed is read straight off `spec`'s
`:degrade` policy (`:on-update`) - see `voodoo.widget/make`.

</main>
</div>
