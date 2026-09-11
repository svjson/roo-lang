---
layout: reference
title: proofread.rules.simple-expressions.redundant-let-rebind
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.redundant-let-rebind.html">proofread.rules.simple-expressions.redundant-let-rebind</a>
<ol>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/all-update-steps-p.html">all-update-steps?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/cond-thread-update-keys.html">cond-thread-update-keys</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/cond-thread-p.html">cond-thread?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/reads-updated-key-p.html">reads-updated-key?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/tests-read-updated-key-p.html">tests-read-updated-key?</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/update-keys.html">update-keys</a></li>
<li><a href="proofread.rules.simple-expressions.redundant-let-rebind/update-step-key.html">update-step-key</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.redundant-let-rebind</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.redundant-let-rebind.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.redundant-let-rebind

| Symbol                                                                                                             | Summary                                                               |
| ------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- |
| [`all-update-steps?`](proofread.rules.simple-expressions.redundant-let-rebind/all-update-steps-p.html)             | Test whether all `cond->` steps from `index` are `update` steps.      |
| [`candidate?`](proofread.rules.simple-expressions.redundant-let-rebind/candidate-p.html)                           | Test whether a self-rebinding `let` step can be folded into the body. |
| [`check`](proofread.rules.simple-expressions.redundant-let-rebind/check.html)                                      | Return a `redundant-let-rebind` diagnostic for matching nodes.        |
| [`cond-thread-update-keys`](proofread.rules.simple-expressions.redundant-let-rebind/cond-thread-update-keys.html)  | Return updated keys for a `cond->` over `name`.                       |
| [`cond-thread?`](proofread.rules.simple-expressions.redundant-let-rebind/cond-thread-p.html)                       | Test whether a node is a non-empty `cond->` over `name`.              |
| [`reads-updated-key?`](proofread.rules.simple-expressions.redundant-let-rebind/reads-updated-key-p.html)           | Test whether a node reads an updated key from `target-name`.          |
| [`tests-read-updated-key?`](proofread.rules.simple-expressions.redundant-let-rebind/tests-read-updated-key-p.html) | Test whether `cond->` tests read a key changed by earlier steps.      |
| [`update-keys`](proofread.rules.simple-expressions.redundant-let-rebind/update-keys.html)                          | Return keyword names updated by `cond->` steps from `index`.          |
| [`update-step-key`](proofread.rules.simple-expressions.redundant-let-rebind/update-step-key.html)                  | Return the updated keyword name from a `cond->` update step.          |

</main>
</div>
