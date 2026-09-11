---
layout: reference
title: voodoo.session/remove!
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
<li><a class="active" href="remove-bang.html">remove!</a></li>
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
<span aria-current="page">remove!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/remove-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>remove!</code></h1>
</header>

Remove a widget from a session, erasing it from the terminal.

<hr class="signature-divider">

```roo
(remove! {:keys [order widgets] :as session} handle)
```

<hr class="signature-divider">

## Signature:

```roo
(remove! {:keys [order widgets] :as session} handle)
```

| Arg                                 | Description                |
| ----------------------------------- | -------------------------- |
| {:keys [order widgets] :as session} |                            |
| handle                              | Handle returned by `add!`. |

### Returns:

`handle`. A no-op (beyond returning `handle`) if `handle` isn't
currently tracked by `session` - e.g. a widget added with an
append-mode `:on-add :untracked` policy while not in a tty.

In tty mode: clears the widget's row(s) - it may span more than one,
see `voodoo.widget/line-count` - then redraws any widgets below it so
they close the gap. If the removed widget was the bottom-most (or
only) one, its rows are cleared using `pr!`, not `prn!` - a
positioning-only escape sequence has no content that a trailing newline
should follow, so the cursor is left exactly at the start of the first
(now blank) row, where the widget used to be. Whatever prints next - by
this session, or, as `pkg/proof` does, by the caller once it removes an
in-progress widget and prints its own final line - lands exactly there
instead of one row too low. In append mode, whether the widget's last
text is printed on its way out is read straight off its `:degrade`
policy (`:on-remove`) - see `voodoo.widget/make`. Either way, bookkeeping
cleanup is delegated to `evict!`.

- `session`: Session returned by `make`.

### Returns:

`handle`. A no-op (beyond returning `handle`) if `handle` isn't
currently tracked by `session` - e.g. a widget added with an
append-mode `:on-add :untracked` policy while not in a tty.

In tty mode: clears the widget's row(s) - it may span more than one,
see `voodoo.widget/line-count` - then redraws any widgets below it so
they close the gap. If the removed widget was the bottom-most (or
only) one, its rows are cleared using `pr!`, not `prn!` - a
positioning-only escape sequence has no content that a trailing newline
should follow, so the cursor is left exactly at the start of the first
(now blank) row, where the widget used to be. Whatever prints next - by
this session, or, as `pkg/proof` does, by the caller once it removes an
in-progress widget and prints its own final line - lands exactly there
instead of one row too low. In append mode, whether the widget's last
text is printed on its way out is read straight off its `:degrade`
policy (`:on-remove`) - see `voodoo.widget/make`. Either way, bookkeeping
cleanup is delegated to `evict!`.

</main>
</div>
