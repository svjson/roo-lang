---
layout: reference
title: proof.core
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.core.html">proof.core</a>
<ol>
<li><a href="proof.core/annotate-tests-bang.html">annotate-tests!</a></li>
<li><a href="proof.core/assert.html">assert</a></li>
<li><a href="proof.core/clear-bang.html">clear!</a></li>
<li><a href="proof.core/current-failures.html">current-failures</a></li>
<li><a href="proof.core/declared-namespace.html">declared-namespace</a></li>
<li><a href="proof.core/deftest.html">deftest</a></li>
<li><a href="proof.core/expect.html">expect</a></li>
<li><a href="proof.core/is.html">is</a></li>
<li><a href="proof.core/record-failure-bang.html">record-failure!</a></li>
<li><a href="proof.core/register-test-bang.html">register-test!</a></li>
<li><a href="proof.core/run.html">run</a></li>
<li><a href="proof.core/run-and-report-selected-test.html">run-and-report-selected-test</a></li>
<li><a href="proof.core/run-selected.html">run-selected</a></li>
<li><a href="proof.core/run-test.html">run-test</a></li>
<li><a href="proof.core/run-test-body.html">run-test-body</a></li>
<li><a href="proof.core/run-test-lifecycle.html">run-test-lifecycle</a></li>
<li><a href="proof.core/should.html">should</a></li>
<li><a href="proof.core/tests.html">tests</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.core</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.core.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.core

| Symbol                                                                         | Summary                                                                    |
| ------------------------------------------------------------------------------ | -------------------------------------------------------------------------- |
| [`annotate-tests!`](proof.core/annotate-tests-bang.html)                       |                                                                            |
| [`assert`](proof.core/assert.html)                                             |                                                                            |
| [`clear!`](proof.core/clear-bang.html)                                         |                                                                            |
| [`current-failures`](proof.core/current-failures.html)                         |                                                                            |
| [`declared-namespace`](proof.core/declared-namespace.html)                     |                                                                            |
| [`deftest`](proof.core/deftest.html)                                           | Defines and registers a test.                                              |
| [`expect`](proof.core/expect.html)                                             |                                                                            |
| [`is`](proof.core/is.html)                                                     | Asserts that an expression is truthy.                                      |
| [`record-failure!`](proof.core/record-failure-bang.html)                       | Record one assertion failure for the test currently executing.             |
| [`register-test!`](proof.core/register-test-bang.html)                         |                                                                            |
| [`run`](proof.core/run.html)                                                   |                                                                            |
| [`run-and-report-selected-test`](proof.core/run-and-report-selected-test.html) |                                                                            |
| [`run-selected`](proof.core/run-selected.html)                                 |                                                                            |
| [`run-test`](proof.core/run-test.html)                                         | Execute TEST and materialize its result.                                   |
| [`run-test-body`](proof.core/run-test-body.html)                               | Execute BODY while translating failures into Proof's body-result contract. |
| [`run-test-lifecycle`](proof.core/run-test-lifecycle.html)                     | Execute one test's optional setup, body, and optional cleanup.             |
| [`should`](proof.core/should.html)                                             |                                                                            |
| [`tests`](proof.core/tests.html)                                               |                                                                            |

</main>
</div>
