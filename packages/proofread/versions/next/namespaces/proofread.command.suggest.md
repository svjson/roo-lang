---
layout: reference
title: proofread.command.suggest
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.command.suggest.html">proofread.command.suggest</a>
<ol>
<li><a href="proofread.command.suggest/checked-diagnostics.html">checked-diagnostics</a></li>
<li><a href="proofread.command.suggest/command.html">command</a></li>
<li><a href="proofread.command.suggest/execute-bang.html">execute!</a></li>
<li><a href="proofread.command.suggest/inner-diagnostic-before-p.html">inner-diagnostic-before?</a></li>
<li><a href="proofread.command.suggest/parse-error-diagnostic.html">parse-error-diagnostic</a></li>
<li><a href="proofread.command.suggest/parse-positive-int.html">parse-positive-int</a></li>
<li><a href="proofread.command.suggest/position-before-or-at-p.html">position-before-or-at?</a></li>
<li><a href="proofread.command.suggest/position-before-p.html">position-before?</a></li>
<li><a href="proofread.command.suggest/suggestions-at.html">suggestions-at</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.command.suggest</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.command.suggest.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.command.suggest

| Symbol                                                                                 | Summary                                                           |
| -------------------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| [`checked-diagnostics`](proofread.command.suggest/checked-diagnostics.html)            |                                                                   |
| [`command`](proofread.command.suggest/command.html)                                    | cli-trooper declaration for Proofread's `suggest` command.        |
| [`execute!`](proofread.command.suggest/execute-bang.html)                              | Print suggestions at a file position from parsed command options. |
| [`inner-diagnostic-before?`](proofread.command.suggest/inner-diagnostic-before-p.html) |                                                                   |
| [`parse-error-diagnostic`](proofread.command.suggest/parse-error-diagnostic.html)      | Build a diagnostic for a parse or read error.                     |
| [`parse-positive-int`](proofread.command.suggest/parse-positive-int.html)              | Convert CLI text to a positive integer.                           |
| [`position-before-or-at?`](proofread.command.suggest/position-before-or-at-p.html)     |                                                                   |
| [`position-before?`](proofread.command.suggest/position-before-p.html)                 |                                                                   |
| [`suggestions-at`](proofread.command.suggest/suggestions-at.html)                      | Return suggestable diagnostics containing a source position.      |

</main>
</div>
