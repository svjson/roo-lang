---
layout: reference
title: proofread.command.check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.command.check.html">proofread.command.check</a>
<ol>
<li><a href="proofread.command.check/await-execution-bang.html">await-execution!</a></li>
<li><a href="proofread.command.check/collect-files-bang.html">collect-files!</a></li>
<li><a href="proofread.command.check/command.html">command</a></li>
<li><a href="proofread.command.check/diagnostic-count.html">diagnostic-count</a></li>
<li><a href="proofread.command.check/execute-bang.html">execute!</a></li>
<li><a href="proofread.command.check/inspect-file-bang.html">inspect-file!</a></li>
<li><a href="proofread.command.check/record-file-result-bang.html">record-file-result!</a></li>
<li><a href="proofread.command.check/validation-errors.html">validation-errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.command.check</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.command.check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.command.check

| Symbol                                                                        | Summary                                                            |
| ----------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`await-execution!`](proofread.command.check/await-execution-bang.html)       | Animate a status widget until a worker execution becomes terminal. |
| [`collect-files!`](proofread.command.check/collect-files-bang.html)           | Discover Proofread inputs on the application worker.               |
| [`command`](proofread.command.check/command.html)                             | cli-trooper declaration for Proofread's `check` command.           |
| [`diagnostic-count`](proofread.command.check/diagnostic-count.html)           | Count diagnostics with a given severity.                           |
| [`execute!`](proofread.command.check/execute-bang.html)                       | Run proofread checks with parsed command options.                  |
| [`inspect-file!`](proofread.command.check/inspect-file-bang.html)             | Inspect one file on the application worker.                        |
| [`record-file-result!`](proofread.command.check/record-file-result-bang.html) | Add a completed file result to a running summary and reporter.     |
| [`validation-errors`](proofread.command.check/validation-errors.html)         | Return semantic diagnostics for parsed check options.              |

</main>
</div>
