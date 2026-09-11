---
layout: reference
title: proofread.inspect.expressions
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.expressions.html">proofread.inspect.expressions</a>
<ol>
<li><a href="proofread.inspect.expressions/default-options.html">default-options</a></li>
<li><a href="proofread.inspect.expressions/default-simple-call-heads.html">default-simple-call-heads</a></li>
<li><a href="proofread.inspect.expressions/literal-p.html">literal?</a></li>
<li><a href="proofread.inspect.expressions/simple-argument-p.html">simple-argument?</a></li>
<li><a href="proofread.inspect.expressions/simple-arguments-p.html">simple-arguments?</a></li>
<li><a href="proofread.inspect.expressions/simple-call-heads.html">simple-call-heads</a></li>
<li><a href="proofread.inspect.expressions/simple-call-p.html">simple-call?</a></li>
<li><a href="proofread.inspect.expressions/simple-p.html">simple?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.expressions</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.expressions.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.expressions

| Symbol                                                                                      | Summary                                                             |
| ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- |
| [`default-options`](proofread.inspect.expressions/default-options.html)                     | Default simple-expression inspection options.                       |
| [`default-simple-call-heads`](proofread.inspect.expressions/default-simple-call-heads.html) | Default call heads treated as simple expressions.                   |
| [`literal?`](proofread.inspect.expressions/literal-p.html)                                  | Test whether an AST node is a simple literal.                       |
| [`simple-argument?`](proofread.inspect.expressions/simple-argument-p.html)                  | Test whether an AST node is simple enough to inline as an argument. |
| [`simple-arguments?`](proofread.inspect.expressions/simple-arguments-p.html)                | Test whether call arguments from `index` are simple.                |
| [`simple-call-heads`](proofread.inspect.expressions/simple-call-heads.html)                 | Return configured simple call heads.                                |
| [`simple-call?`](proofread.inspect.expressions/simple-call-p.html)                          | Test whether an AST node is a simple call expression.               |
| [`simple?`](proofread.inspect.expressions/simple-p.html)                                    | Test whether an AST node is simple enough to inline.                |

</main>
</div>
