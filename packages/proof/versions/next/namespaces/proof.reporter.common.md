---
layout: reference
title: proof.reporter.common
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.reporter.common.html">proof.reporter.common</a>
<ol>
<li><a href="proof.reporter.common/dark-red.html">dark-red</a></li>
<li><a href="proof.reporter.common/duration-args.html">duration-args</a></li>
<li><a href="proof.reporter.common/highlight-test-name.html">highlight-test-name</a></li>
<li><a href="proof.reporter.common/pad-status-word.html">pad-status-word</a></li>
<li><a href="proof.reporter.common/padded-status-label.html">padded-status-label</a></li>
<li><a href="proof.reporter.common/status-column-width.html">status-column-width</a></li>
<li><a href="proof.reporter.common/status-label.html">status-label</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.reporter.common</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.reporter.common.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.reporter.common

| Symbol                                                                  | Summary                                                                                                                                                                                                                                          |
| ----------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [`dark-red`](proof.reporter.common/dark-red.html)                       |                                                                                                                                                                                                                                                  |
| [`duration-args`](proof.reporter.common/duration-args.html)             |                                                                                                                                                                                                                                                  |
| [`highlight-test-name`](proof.reporter.common/highlight-test-name.html) |                                                                                                                                                                                                                                                  |
| [`pad-status-word`](proof.reporter.common/pad-status-word.html)         | Right-justify WORD to `status-column-width` with leading spaces.                                                                                                                                                                                 |
| [`padded-status-label`](proof.reporter.common/padded-status-label.html) |                                                                                                                                                                                                                                                  |
| [`status-column-width`](proof.reporter.common/status-column-width.html) | Width of the status column in the simple reporter's output: every<br>status word (RUNNING, PASS, FAIL, ERROR) right-justified to this width<br>lines up in a single column. Driven by "RUNNING", the longest of<br>them, plus one leading space. |
| [`status-label`](proof.reporter.common/status-label.html)               |                                                                                                                                                                                                                                                  |

</main>
</div>
