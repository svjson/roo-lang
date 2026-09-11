---
layout: reference
title: proof.reporter.simple/end-test
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.reporter.simple.html">proof.reporter.simple</a>
<ol>
<li><a href="begin-test.html">begin-test</a></li>
<li><a class="active" href="end-test.html">end-test</a></li>
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
<span aria-current="page">end-test</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.reporter.simple/end-test.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.reporter.simple.html">proof.reporter.simple</a>/</div>
<h1><code>end-test</code></h1>
</header>

Report TEST's finished RESULT.

<hr class="signature-divider">

```roo
(end-test state context test result options)
```

<hr class="signature-divider">

Removes TEST's in-progress widget (a no-op in append mode, where it was
never tracked) and prints the finished line directly, no voodoo
involved - the one line append mode was always going to show.

<hr class="signature-divider">

## Signature:

```roo
(end-test state context test result options)
```

| Arg     | Description                                                |
| ------- | ---------------------------------------------------------- |
| state   | Reporter state, unused - the simple reporter is stateless. |
| context | Value from `begin-test` - the session/handle to remove.    |
| test    | Test that ran, unused.                                     |
| result  | Its result.                                                |
| options | Run options, e.g. `:durations?`.                           |

### Returns:

A map with `:state` (unchanged).

</main>
</div>
