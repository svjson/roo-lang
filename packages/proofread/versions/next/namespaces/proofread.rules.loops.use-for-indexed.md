---
layout: reference
title: proofread.rules.loops.use-for-indexed
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.loops.use-for-indexed.html">proofread.rules.loops.use-for-indexed</a>
<ol>
<li><a href="proofread.rules.loops.use-for-indexed/candidate-p.html">candidate?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/check.html">check</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/contains-indexed-nth-p.html">contains-indexed-nth?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/count-condition-target.html">count-condition-target</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/counted-while-p.html">counted-while?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/final-form.html">final-form</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/increment-final-body-p.html">increment-final-body?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/increment-p.html">increment?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/indexed-nth-p.html">indexed-nth?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/one-node-p.html">one-node?</a></li>
<li><a href="proofread.rules.loops.use-for-indexed/single-symbol-vector-p.html">single-symbol-vector?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.loops.use-for-indexed</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.loops.use-for-indexed.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.loops.use-for-indexed

| Symbol                                                                                        | Summary                                                            |
| --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`candidate?`](proofread.rules.loops.use-for-indexed/candidate-p.html)                        | Test whether a let-wrapped while loop should use `for-indexed`.    |
| [`check`](proofread.rules.loops.use-for-indexed/check.html)                                   | Return a `use-for-indexed` diagnostic for matching nodes.          |
| [`contains-indexed-nth?`](proofread.rules.loops.use-for-indexed/contains-indexed-nth-p.html)  | Test whether a node contains an indexed read from `collection`.    |
| [`count-condition-target`](proofread.rules.loops.use-for-indexed/count-condition-target.html) | Return the counted collection from `(< index (count collection))`. |
| [`counted-while?`](proofread.rules.loops.use-for-indexed/counted-while-p.html)                | Test whether a while loop manually indexes one collection.         |
| [`final-form`](proofread.rules.loops.use-for-indexed/final-form.html)                         | Return the final executable form inside `node`.                    |
| [`increment-final-body?`](proofread.rules.loops.use-for-indexed/increment-final-body-p.html)  | Test whether a while body ends by incrementing `index-name`.       |
| [`increment?`](proofread.rules.loops.use-for-indexed/increment-p.html)                        | Test whether a node increments `index-name` by one.                |
| [`indexed-nth?`](proofread.rules.loops.use-for-indexed/indexed-nth-p.html)                    | Test whether a node reads `collection` with `index-name`.          |
| [`one-node?`](proofread.rules.loops.use-for-indexed/one-node-p.html)                          | Test whether an AST node is the number `1`.                        |
| [`single-symbol-vector?`](proofread.rules.loops.use-for-indexed/single-symbol-vector-p.html)  | Test whether a vector contains one expected symbol.                |

</main>
</div>
