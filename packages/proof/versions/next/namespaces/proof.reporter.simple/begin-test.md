---
layout: reference
title: proof.reporter.simple/begin-test
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.simple.html">proof.reporter.simple</a>
<ol>
<li><a class="active" href="begin-test.html">begin-test</a></li>
<li><a href="end-test.html">end-test</a></li>
<li><a href="make.html">make</a></li>
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
<a href="../proof.reporter.simple.html">proof.reporter.simple</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">begin-test</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.simple/begin-test.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.simple.html">proof.reporter.simple</a>/</div>
<h1><code>begin-test</code></h1>
</header>

Report that TEST is about to run.

<hr class="signature-divider">

```roo
(begin-test state test options)
```

<hr class="signature-divider">

Shows TEST's in-progress line as its own single-widget voodoo session
(tty only - `voodoo.widget/tty-only` - so append mode never sees it and
never tracks it).

<hr class="signature-divider">

## Signature:

```roo
(begin-test state test options)
```

| Arg     | Description                                                |
| ------- | ---------------------------------------------------------- |
| state   | Reporter state, unused - the simple reporter is stateless. |
| test    | Test about to run.                                         |
| options | Run options, e.g. `:tty?`.                                 |

### Returns:

A map with `:state` (unchanged) and `:context` (the session and handle
backing TEST's in-progress line, threaded into `end-test`).

</main>
</div>
