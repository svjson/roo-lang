---
layout: reference
title: proof.reporter.tree
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.reporter.tree.html">proof.reporter.tree</a>
<ol>
<li><a href="proof.reporter.tree/add-pending-widgets-bang.html">add-pending-widgets!</a></li>
<li><a href="proof.reporter.tree/begin-test.html">begin-test</a></li>
<li><a href="proof.reporter.tree/bullet-for-index.html">bullet-for-index</a></li>
<li><a href="proof.reporter.tree/end-test.html">end-test</a></li>
<li><a href="proof.reporter.tree/make.html">make</a></li>
<li><a href="proof.reporter.tree/pending-text.html">pending-text</a></li>
<li><a href="proof.reporter.tree/result-text.html">result-text</a></li>
<li><a href="proof.reporter.tree/running-text.html">running-text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.reporter.tree</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.reporter.tree.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.reporter.tree

| Symbol                                                                      | Summary                                                                                                                                                                                                                                                                                                             |
| --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`add-pending-widgets!`](proof.reporter.tree/add-pending-widgets-bang.html) | Add every test in TESTS to WIDGET-SESSION as a pending widget, in file<br>order.                                                                                                                                                                                                                                    |
| [`begin-test`](proof.reporter.tree/begin-test.html)                         | Report that TEST is about to run.                                                                                                                                                                                                                                                                                   |
| [`bullet-for-index`](proof.reporter.tree/bullet-for-index.html)             | The bullet character for the INDEX'th (1-based) test out of TOTAL in<br>its file.                                                                                                                                                                                                                                   |
| [`end-test`](proof.reporter.tree/end-test.html)                             | Report TEST's finished RESULT.                                                                                                                                                                                                                                                                                      |
| [`make`](proof.reporter.tree/make.html)                                     |                                                                                                                                                                                                                                                                                                                     |
| [`pending-text`](proof.reporter.tree/pending-text.html)                     | Render TEST's placeholder line: BULLET, a blank status-column-width<br>field where its status word will go, and TEST's name - the whole thing<br>(other than BULLET) grayed out to mark it as not started.                                                                                                          |
| [`result-text`](proof.reporter.tree/result-text.html)                       | Render RESULT's finished line for TEST's BULLET: the same shape as<br>`running-text`'s but with RESULT's real status/duration, folded<br>together with any failure or error details into one multi-line string -<br>see `voodoo.widget/line-count` - so they finalize, redraw, and get evicted<br>as a single unit. |
| [`running-text`](proof.reporter.tree/running-text.html)                     | Render TEST's in-progress line: BULLET, a grayed RUNNING status word,<br>and TEST's name.                                                                                                                                                                                                                           |

</main>
</div>
