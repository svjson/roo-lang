---
layout: reference
title: proofread.rules.redundant-functions.redundant-function
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.redundant-functions.redundant-function.html">proofread.rules.redundant-functions.redundant-function</a>
<ol>
<li><a href="proofread.rules.redundant-functions.redundant-function/check.html">check</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/closed-over-symbol-p.html">closed-over-symbol?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/closed-over-wrapper-arguments-p.html">closed-over-wrapper-arguments?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/closed-over-wrapper-p.html">closed-over-wrapper?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/default-options.html">default-options</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/defun-body-index.html">defun-body-index</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/directly-redundant-predicate-call-p.html">directly-redundant-predicate-call?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/literal-node-p.html">literal-node?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/parameter-node-p.html">parameter-node?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/plain-param-names.html">plain-param-names</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/redundant-function-p.html">redundant-function?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/redundant-keyword-accessor-p.html">redundant-keyword-accessor?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/skipped-function-p.html">skipped-function?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/specialized-wrapper-p.html">specialized-wrapper?</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/unqualified-name.html">unqualified-name</a></li>
<li><a href="proofread.rules.redundant-functions.redundant-function/wrapper-arguments-p.html">wrapper-arguments?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.redundant-functions.redundant-function</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.redundant-functions.redundant-function.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.redundant-functions.redundant-function

| Symbol                                                                                                                                  | Summary                                                                      |
| --------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| [`check`](proofread.rules.redundant-functions.redundant-function/check.html)                                                            | Return a `redundant-function` diagnostic for matching nodes.                 |
| [`closed-over-symbol?`](proofread.rules.redundant-functions.redundant-function/closed-over-symbol-p.html)                               | Test whether an AST node is a direct non-parameter symbol.                   |
| [`closed-over-wrapper-arguments?`](proofread.rules.redundant-functions.redundant-function/closed-over-wrapper-arguments-p.html)         | Test whether wrapper arguments are simple and use a closed-over symbol.      |
| [`closed-over-wrapper?`](proofread.rules.redundant-functions.redundant-function/closed-over-wrapper-p.html)                             | Test whether a function only forwards to another call with closed-over data. |
| [`default-options`](proofread.rules.redundant-functions.redundant-function/default-options.html)                                        | Return the default redundant-function configuration.                         |
| [`defun-body-index`](proofread.rules.redundant-functions.redundant-function/defun-body-index.html)                                      | Return the first body child index for a `defun` form.                        |
| [`directly-redundant-predicate-call?`](proofread.rules.redundant-functions.redundant-function/directly-redundant-predicate-call-p.html) | Test whether an AST node is a directly redundant predicate call.             |
| [`literal-node?`](proofread.rules.redundant-functions.redundant-function/literal-node-p.html)                                           | Test whether an AST node is a literal value.                                 |
| [`parameter-node?`](proofread.rules.redundant-functions.redundant-function/parameter-node-p.html)                                       | Test whether an AST node is one of the function parameters.                  |
| [`plain-param-names`](proofread.rules.redundant-functions.redundant-function/plain-param-names.html)                                    | Return a plain symbol parameter name vector.                                 |
| [`redundant-function?`](proofread.rules.redundant-functions.redundant-function/redundant-function-p.html)                               | Test whether a `defun` only names a simple redundant expression.             |
| [`redundant-keyword-accessor?`](proofread.rules.redundant-functions.redundant-function/redundant-keyword-accessor-p.html)               | Test whether an AST node redundantly names a keyword accessor.               |
| [`skipped-function?`](proofread.rules.redundant-functions.redundant-function/skipped-function-p.html)                                   | Test whether a function name is exempt from redundant-function checks.       |
| [`specialized-wrapper?`](proofread.rules.redundant-functions.redundant-function/specialized-wrapper-p.html)                             | Test whether an AST node is a specialized predicate wrapper call.            |
| [`unqualified-name`](proofread.rules.redundant-functions.redundant-function/unqualified-name.html)                                      | Return the name after a namespace slash.                                     |
| [`wrapper-arguments?`](proofread.rules.redundant-functions.redundant-function/wrapper-arguments-p.html)                                 | Test whether call arguments only specialize parameters with literals.        |

</main>
</div>
