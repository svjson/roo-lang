---
layout: reference
title: proof.reporter.tree/begin-test
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.tree.html">proof.reporter.tree</a>
<ol>
<li><a href="add-pending-widgets-bang.html">add-pending-widgets!</a></li>
<li><a class="active" href="begin-test.html">begin-test</a></li>
<li><a href="bullet-for-index.html">bullet-for-index</a></li>
<li><a href="end-test.html">end-test</a></li>
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
<span aria-current="page">begin-test</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.tree/begin-test.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.tree.html">proof.reporter.tree</a>/</div>
<h1><code>begin-test</code></h1>
</header>

Report that TEST is about to run.

<hr class="signature-divider">

```roo
(begin-test state test options)
```

<hr class="signature-divider">

When TEST starts a new file: prints the file's header line, opens a
fresh voodoo session scoped to that file - the previous file's session,
if any, is simply never referenced again, so its rows stay exactly as
last drawn - and eagerly adds every test in the file as a pending
widget, so the whole file's tree is visible before any of its tests
run. Either way, TEST's own widget then transitions from pending (or,
for a file's first test, straight from being just added) to running.

<hr class="signature-divider">

## Signature:

```roo
(begin-test state test options)
```

| Arg     | Description                                                                                                                                                                                            |
| ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| state   | Reporter state - `:current-file`, `:indexes`, `:counts`,<br>`:tests-by-file`, `:session` (current file's voodoo session), and<br>`:handles` (that file's widget handles, aligned with its test order). |
| test    | Test about to run.                                                                                                                                                                                     |
| options | Run options, e.g. `:tty?`.                                                                                                                                                                             |

### Returns:

A map with `:state` (file/index/session bookkeeping advanced for TEST)
and `:context` (the bullet character, session, and widget handle
TEST's line uses, threaded into `end-test`).

</main>
</div>
