---
layout: reference
title: proof.core/deftest
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
<li><a class="active" href="deftest.html">deftest</a></li>
<li><a href="expect.html">expect</a></li>
<li><a href="is.html">is</a></li>
<li><a href="record-failure-bang.html">record-failure!</a></li>
<li><a href="register-test-bang.html">register-test!</a></li>
<li><a href="run.html">run</a></li>
<li><a href="run-and-report-selected-test.html">run-and-report-selected-test</a></li>
<li><a href="run-selected.html">run-selected</a></li>
<li><a href="run-test.html">run-test</a></li>
<li><a href="run-test-body.html">run-test-body</a></li>
<li><a href="run-test-lifecycle.html">run-test-lifecycle</a></li>
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
<span aria-current="page">deftest</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.core/deftest.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.core.html">proof.core</a>/</div>
<h1><code>deftest</code></h1>
<div class="symbol-metadata"><span>See also: <code>is</code>, <code>proof.scenario/given</code>, <code>proof.scenario/when</code>, <code>proof.scenario/then</code></span></div>
</header>

Defines and registers a test.



The body runs when the test suite selects the test. An optional directive map
may supply per-test `:before` and `:after` forms, followed by a fixture vector
(which may be empty). A fixture vector may also follow the name directly.
Bare function symbols and literal lambdas are invoked with zero arguments;
other directive forms are evaluated inline.
Scenario tests may use
`proof.scenario/given`, `proof.scenario/when`, and `proof.scenario/then` as
top-level body phases.

- `name`: Symbol that identifies the test.
- `directives`: Optional map containing `:before` and/or `:after` forms.
- `fixtures`: Optional vector of declared fixture specs.
- `body...`: One or more forms evaluated as the test body.

### Examples:

```roo
(deftest addition
(is (= 4 (+ 2 2))))

(deftest service/responds
{:before start-service!
:after stop-service!}
[]
(is (= :ok (request-status))))
```



</main>
</div>
