---
layout: reference
title: proofread.rules.loops.use-reduce
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.loops.use-reduce.html">proofread.rules.loops.use-reduce</a>
<ol>
<li><a href="proofread.rules.loops.use-reduce/binding-names.html">binding-names</a></li>
<li><a href="proofread.rules.loops.use-reduce/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.loops.use-reduce/check.html">check</a></li>
<li><a href="proofread.rules.loops.use-reduce/collection-loop-p.html">collection-loop?</a></li>
<li><a href="proofread.rules.loops.use-reduce/contains-mutating-set-p.html">contains-mutating-set?</a></li>
<li><a href="proofread.rules.loops.use-reduce/loop-heads.html">loop-heads</a></li>
<li><a href="proofread.rules.loops.use-reduce/loop-mutates-bound-name-p.html">loop-mutates-bound-name?</a></li>
<li><a href="proofread.rules.loops.use-reduce/mutates-bound-name-p.html">mutates-bound-name?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.loops.use-reduce</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.loops.use-reduce.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.loops.use-reduce

| Symbol                                                                                        | Summary                                                   |
| --------------------------------------------------------------------------------------------- | --------------------------------------------------------- |
| [`binding-names`](proofread.rules.loops.use-reduce/binding-names.html)                        | Return direct symbol names from let bindings.             |
| [`candidate?`](proofread.rules.loops.use-reduce/candidate-p.html)                             | Test whether a `let` contains loop-carried mutable state. |
| [`check`](proofread.rules.loops.use-reduce/check.html)                                        | Return a `use-reduce` diagnostic for matching nodes.      |
| [`collection-loop?`](proofread.rules.loops.use-reduce/collection-loop-p.html)                 | Test whether an AST node loops over an input collection.  |
| [`contains-mutating-set?`](proofread.rules.loops.use-reduce/contains-mutating-set-p.html)     | Test whether an AST subtree mutates one of `names`.       |
| [`loop-heads`](proofread.rules.loops.use-reduce/loop-heads.html)                              | Forms that execute a body once per input.                 |
| [`loop-mutates-bound-name?`](proofread.rules.loops.use-reduce/loop-mutates-bound-name-p.html) | Test whether a collection loop mutates one of `names`.    |
| [`mutates-bound-name?`](proofread.rules.loops.use-reduce/mutates-bound-name-p.html)           | Test whether a `set!` form mutates one of `names`.        |

</main>
</div>
