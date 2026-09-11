---
layout: reference
title: proof.core/run-test-lifecycle
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.core.html">proof.core</a>
<ol>
<li><a href="annotate-tests-bang.html">annotate-tests!</a></li>
<li><a href="assert.html">assert</a></li>
<li><a href="clear-bang.html">clear!</a></li>
<li><a href="current-failures.html">current-failures</a></li>
<li><a href="declared-namespace.html">declared-namespace</a></li>
<li><a href="deftest.html">deftest</a></li>
<li><a href="expect.html">expect</a></li>
<li><a href="is.html">is</a></li>
<li><a href="record-failure-bang.html">record-failure!</a></li>
<li><a href="register-test-bang.html">register-test!</a></li>
<li><a href="run.html">run</a></li>
<li><a href="run-and-report-selected-test.html">run-and-report-selected-test</a></li>
<li><a href="run-selected.html">run-selected</a></li>
<li><a href="run-test.html">run-test</a></li>
<li><a href="run-test-body.html">run-test-body</a></li>
<li><a class="active" href="run-test-lifecycle.html">run-test-lifecycle</a></li>
<li><a href="should.html">should</a></li>
<li><a href="tests.html">tests</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.core.html">proof.core</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">run-test-lifecycle</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.core/run-test-lifecycle.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.core.html">proof.core</a>/</div>
<h1><code>run-test-lifecycle</code></h1>
</header>

Execute one test's optional setup, body, and optional cleanup.

<hr class="signature-divider">

```roo
(run-test-lifecycle before body after)
```

<hr class="signature-divider">

Setup and cleanup return values are ignored. A setup error or aborting
assertion prevents BODY from running. AFTER is still attempted exactly once;
its errors are retained separately and do not alter the primary test status.

<hr class="signature-divider">

## Signature:

```roo
(run-test-lifecycle before body after)
```

| Arg    | Description                                                 |
| ------ | ----------------------------------------------------------- |
| before | Optional zero-argument executable run before BODY.          |
| body   | Zero-argument executable containing the test body.          |
| after  | Optional zero-argument executable run after setup and BODY. |

### Returns:

An internal lifecycle envelope containing the primary body result and, when
cleanup raises an error, an `:after {:error ...}` diagnostic.

</main>
</div>
