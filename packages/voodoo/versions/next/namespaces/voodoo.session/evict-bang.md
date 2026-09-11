---
layout: reference
title: voodoo.session/evict!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.session.html">voodoo.session</a>
<ol>
<li><a href="add-bang.html">add!</a></li>
<li><a href="clear-rows-upward.html">clear-rows-upward</a></li>
<li><a href="cursor-baseline-distance.html">cursor-baseline-distance</a></li>
<li><a href="dispatch-bang.html">dispatch!</a></li>
<li><a class="active" href="evict-bang.html">evict!</a></li>
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
<span aria-current="page">evict!</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.session/evict-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.session.html">voodoo.session</a>/</div>
<h1><code>evict!</code></h1>
</header>

Drop a widget from session bookkeeping without altering rendered content.

<hr class="signature-divider">

```roo
(evict! session handle)
```

<hr class="signature-divider">

Use once a widget's last `update!` has already given it its permanent,
final text: the widget stays on screen exactly as drawn - this only
stops `session` from including it in future `:widgets`/`:order`
redraws (e.g. of other widgets below it), so a long-lived block of many
widgets doesn't keep re-touching everything that's already finished as
more of them finalize. Particularly relevant for a widget with
multi-line `:text` (e.g. a failing test's result line folded together
with its failure-detail lines - see `voodoo.widget/line-count`): left
tracked, every later widget's `update!` would keep redrawing all of
that content too.

<hr class="signature-divider">

## Signature:

```roo
(evict! session handle)
```

| Arg     | Description                 |
| ------- | --------------------------- |
| session | Session returned by `make`. |
| handle  | Handle returned by `add!`.  |

### Returns:

`handle`, after removing it from the session's bookkeeping.

</main>
</div>
