---
layout: reference
title: proofread.inspect.sequences
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.sequences.html">proofread.inspect.sequences</a>
<ol>
<li><a href="proofread.inspect.sequences/filter-mapped-nil-results-p.html">filter-mapped-nil-results?</a></li>
<li><a href="proofread.inspect.sequences/nil-filter-p.html">nil-filter?</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-call-values.html">nil-tolerant-call-values</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-map-call-rules.html">nil-tolerant-map-call-rules</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-map-call-values.html">nil-tolerant-map-call-values</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-seq-binding-rules.html">nil-tolerant-seq-binding-rules</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-seq-binding-values.html">nil-tolerant-seq-binding-values</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-seq-call-rules.html">nil-tolerant-seq-call-rules</a></li>
<li><a href="proofread.inspect.sequences/nil-tolerant-seq-call-values.html">nil-tolerant-seq-call-values</a></li>
<li><a href="proofread.inspect.sequences/not-nil-param-p.html">not-nil-param?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.sequences</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.sequences.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.sequences

| Symbol                                                                                                | Summary                                                             |
| ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- |
| [`filter-mapped-nil-results?`](proofread.inspect.sequences/filter-mapped-nil-results-p.html)          | Test whether a filter discards nil results from a map.              |
| [`nil-filter?`](proofread.inspect.sequences/nil-filter-p.html)                                        | Test whether a filter predicate keeps non-nil values.               |
| [`nil-tolerant-call-values`](proofread.inspect.sequences/nil-tolerant-call-values.html)               | Return call arguments selected by nil-tolerant collection rules.    |
| [`nil-tolerant-map-call-rules`](proofread.inspect.sequences/nil-tolerant-map-call-rules.html)         | Maps call names to nil-tolerant map argument selectors.             |
| [`nil-tolerant-map-call-values`](proofread.inspect.sequences/nil-tolerant-map-call-values.html)       | Return call arguments that treat nil as an empty map.               |
| [`nil-tolerant-seq-binding-rules`](proofread.inspect.sequences/nil-tolerant-seq-binding-rules.html)   | Maps binding form names to the first nil-tolerant collection index. |
| [`nil-tolerant-seq-binding-values`](proofread.inspect.sequences/nil-tolerant-seq-binding-values.html) | Return collection expressions from nil-tolerant sequence bindings.  |
| [`nil-tolerant-seq-call-rules`](proofread.inspect.sequences/nil-tolerant-seq-call-rules.html)         | Maps call names to nil-tolerant sequence argument selectors.        |
| [`nil-tolerant-seq-call-values`](proofread.inspect.sequences/nil-tolerant-seq-call-values.html)       | Return call arguments that treat nil as an empty sequence.          |
| [`not-nil-param?`](proofread.inspect.sequences/not-nil-param-p.html)                                  | Test whether an AST node rejects nil values for a parameter.        |

</main>
</div>
