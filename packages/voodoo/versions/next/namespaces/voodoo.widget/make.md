---
layout: reference
title: voodoo.widget/make
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.widget.html">voodoo.widget</a>
<ol>
<li><a href="count-char.html">count-char</a></li>
<li><a href="line-count.html">line-count</a></li>
<li><a class="active" href="make.html">make</a></li>
<li><a href="replaced.html">replaced</a></li>
<li><a href="silent.html">silent</a></li>
<li><a href="tty-only.html">tty-only</a></li>
<li><a href="visible-once.html">visible-once</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.widget.html">voodoo.widget</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">make</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.widget/make.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.widget.html">voodoo.widget</a>/</div>
<h1><code>make</code></h1>
</header>

Build a widget spec with a complete append-mode degrade policy.

<hr class="signature-divider">

```roo
(make opts)
```

<hr class="signature-divider">

Every widget needs a `:degrade` policy telling the session what to do
in append (non-tty) mode at each lifecycle point, but most widgets only
differ from `visible-once` at one or two of those points. `make` lets a
caller state just the difference: any `:degrade` keys given in `opts`
override `visible-once` for those keys only, so
`{:degrade {:on-update :visible}}` yields a full policy identical to
`visible-once` except for `:on-update`. Omitting `:degrade` entirely
yields `visible-once` unchanged.

<hr class="signature-divider">

## Signature:

```roo
(make opts)
```

| Arg  | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| opts | Widget fields, e.g. `{:type :status :text "RUNNING x"}`.<br>An interactive widget may provide<br>`:cursor {:row row :text-before text}` within its rendered `:text`, where<br>`:row` is zero-based and `:text-before` is the rendered part of that row<br>preceding the cursor. A focused widget's session places the terminal cursor<br>there after rendering. Replaying the rendered prefix lets the terminal<br>account for Unicode display width and ANSI sequences itself.<br>`:degrade`, if given, is a partial or full policy map with<br>`:on-add`/`:on-update`/`:on-remove` keys, each `:visible` or<br>`:hidden` (`:on-add` also accepts `:untracked` - see `tty-only`) -<br>see `visible-once`, `silent`, `replaced`, and `tty-only` for<br>ready-made policies. |

### Returns:

A widget, with a complete `:degrade` policy. The session and reporters
read `:text` and `:degrade` straight off it via plain keyword access -
no widget-aware accessor functions needed downstream.

</main>
</div>
