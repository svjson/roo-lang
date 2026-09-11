---
layout: reference
title: proof.reporter.tree/end-test
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.tree.html">proof.reporter.tree</a>
<ol>
<li><a href="add-pending-widgets-bang.html">add-pending-widgets!</a></li>
<li><a href="begin-test.html">begin-test</a></li>
<li><a href="bullet-for-index.html">bullet-for-index</a></li>
<li><a class="active" href="end-test.html">end-test</a></li>
<li><a href="make.html">make</a></li>
<li><a href="pending-text.html">pending-text</a></li>
<li><a href="result-text.html">result-text</a></li>
<li><a href="running-text.html">running-text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.reporter.tree.html">proof.reporter.tree</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">end-test</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.tree/end-test.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.tree.html">proof.reporter.tree</a>/</div>
<h1><code>end-test</code></h1>
</header>

Report TEST's finished RESULT.

<hr class="signature-divider">

```roo
(end-test state context test result options)
```

<hr class="signature-divider">

Folds RESULT's failure/error detail lines into the same widget as its
main result line, updates it there (the one `:on-update :visible`
update - see `voodoo.widget/replaced` for the general shape), then
evicts that widget from the session's live tracking: its permanent
text has been drawn, and later tests in the file shouldn't keep
redrawing (or re-checking) content that will never change again.

<hr class="signature-divider">

## Signature:

```roo
(end-test state context test result options)
```

| Arg     | Description                                                                               |
| ------- | ----------------------------------------------------------------------------------------- |
| state   | Reporter state, unused - already advanced by `begin-test`.                                |
| context | Value from `begin-test` - the bullet character, session,<br>and handle for TEST's widget. |
| test    | Test that ran, unused - `context` already has what's needed.                              |
| result  | Its result.                                                                               |
| options | Run options, e.g. `:durations?`.                                                          |

### Returns:

A map with `:state` (unchanged).

</main>
</div>
