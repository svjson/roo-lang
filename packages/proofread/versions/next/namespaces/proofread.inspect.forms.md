---
layout: reference
title: proofread.inspect.forms
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.forms.html">proofread.inspect.forms</a>
<ol>
<li><a href="proofread.inspect.forms/branch-predicate-position-p.html">branch-predicate-position?</a></li>
<li><a href="proofread.inspect.forms/call-p.html">call?</a></li>
<li><a href="proofread.inspect.forms/cond-test-child-p.html">cond-test-child?</a></li>
<li><a href="proofread.inspect.forms/count-argument.html">count-argument</a></li>
<li><a href="proofread.inspect.forms/count-call-p.html">count-call?</a></li>
<li><a href="proofread.inspect.forms/direct-symbol-p.html">direct-symbol?</a></li>
<li><a href="proofread.inspect.forms/else-child-p.html">else-child?</a></li>
<li><a href="proofread.inspect.forms/empty-fallback-p.html">empty-fallback?</a></li>
<li><a href="proofread.inspect.forms/empty-map-p.html">empty-map?</a></li>
<li><a href="proofread.inspect.forms/empty-vector-p.html">empty-vector?</a></li>
<li><a href="proofread.inspect.forms/fn-single-param-name.html">fn-single-param-name</a></li>
<li><a href="proofread.inspect.forms/form-sequence-string.html">form-sequence-string</a></li>
<li><a href="proofread.inspect.forms/form-string.html">form-string</a></li>
<li><a href="proofread.inspect.forms/full-if-p.html">full-if?</a></li>
<li><a href="proofread.inspect.forms/head-name.html">head-name</a></li>
<li><a href="proofread.inspect.forms/if-else.html">if-else</a></li>
<li><a href="proofread.inspect.forms/keyword-lookup-key-name.html">keyword-lookup-key-name</a></li>
<li><a href="proofread.inspect.forms/keyword-lookup-p.html">keyword-lookup?</a></li>
<li><a href="proofread.inspect.forms/keyword-name.html">keyword-name</a></li>
<li><a href="proofread.inspect.forms/literal-number-p.html">literal-number?</a></li>
<li><a href="proofread.inspect.forms/literal-string-with-len-p.html">literal-string-with-len?</a></li>
<li><a href="proofread.inspect.forms/literal-string-p.html">literal-string?</a></li>
<li><a href="proofread.inspect.forms/map-form-p.html">map-form?</a></li>
<li><a href="proofread.inspect.forms/nil-node-p.html">nil-node?</a></li>
<li><a href="proofread.inspect.forms/predicate-position-p.html">predicate-position?</a></li>
<li><a href="proofread.inspect.forms/same-form-p.html">same-form?</a></li>
<li><a href="proofread.inspect.forms/single-binding-symbol.html">single-binding-symbol</a></li>
<li><a href="proofread.inspect.forms/sole-let-body-form.html">sole-let-body-form</a></li>
<li><a href="proofread.inspect.forms/symbol-name.html">symbol-name</a></li>
<li><a href="proofread.inspect.forms/thread-first-call-p.html">thread-first-call?</a></li>
<li><a href="proofread.inspect.forms/thread-first-form-p.html">thread-first-form?</a></li>
<li><a href="proofread.inspect.forms/thread-first-transform-p.html">thread-first-transform?</a></li>
<li><a href="proofread.inspect.forms/zero-node-p.html">zero-node?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.forms</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.forms.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.forms

| Symbol                                                                                   | Summary                                                                 |
| ---------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| [`branch-predicate-position?`](proofread.inspect.forms/branch-predicate-position-p.html) | Test whether an AST node is used as a branch predicate.                 |
| [`call?`](proofread.inspect.forms/call-p.html)                                           | Test whether an AST node is a list call with a specific head and arity. |
| [`cond-test-child?`](proofread.inspect.forms/cond-test-child-p.html)                     | Test whether a node is a condition child in a `cond` form.              |
| [`count-argument`](proofread.inspect.forms/count-argument.html)                          | Return the argument expression of a `(count value)` call.               |
| [`count-call?`](proofread.inspect.forms/count-call-p.html)                               | Test whether an AST node is a two-argument `(count ...)` call.          |
| [`direct-symbol?`](proofread.inspect.forms/direct-symbol-p.html)                         | Test whether an AST node is a direct symbol reference.                  |
| [`else-child?`](proofread.inspect.forms/else-child-p.html)                               | Test whether `node` is the else branch of `parent`.                     |
| [`empty-fallback?`](proofread.inspect.forms/empty-fallback-p.html)                       | Test whether an AST node is an empty fallback value.                    |
| [`empty-map?`](proofread.inspect.forms/empty-map-p.html)                                 | Test whether an AST node is an empty map literal.                       |
| [`empty-vector?`](proofread.inspect.forms/empty-vector-p.html)                           | Test whether an AST node is an empty vector literal.                    |
| [`fn-single-param-name`](proofread.inspect.forms/fn-single-param-name.html)              | Return the parameter name from a one-argument `fn`.                     |
| [`form-sequence-string`](proofread.inspect.forms/form-sequence-string.html)              | Render form nodes as a single space-separated string.                   |
| [`form-string`](proofread.inspect.forms/form-string.html)                                | Return the source-like string for an AST node form.                     |
| [`full-if?`](proofread.inspect.forms/full-if-p.html)                                     | Test whether an AST node is an `if` form with an explicit else branch.  |
| [`head-name`](proofread.inspect.forms/head-name.html)                                    | Return the list head name for an AST node.                              |
| [`if-else`](proofread.inspect.forms/if-else.html)                                        | Return the else branch of a full `if` form.                             |
| [`keyword-lookup-key-name`](proofread.inspect.forms/keyword-lookup-key-name.html)        | Return the key name from a keyword lookup form.                         |
| [`keyword-lookup?`](proofread.inspect.forms/keyword-lookup-p.html)                       | Test whether an AST node is a keyword lookup form.                      |
| [`keyword-name`](proofread.inspect.forms/keyword-name.html)                              | Return the keyword name for an AST node.                                |
| [`literal-number?`](proofread.inspect.forms/literal-number-p.html)                       | Test whether a node is a numeric literal.                               |
| [`literal-string-with-len?`](proofread.inspect.forms/literal-string-with-len-p.html)     | Test whether a node is a string literal with the given length.          |
| [`literal-string?`](proofread.inspect.forms/literal-string-p.html)                       | Test whether a node is a string literal.                                |
| [`map-form?`](proofread.inspect.forms/map-form-p.html)                                   | Test whether an AST node is a single-sequence `map` form.               |
| [`nil-node?`](proofread.inspect.forms/nil-node-p.html)                                   | Test whether an AST node is the symbol `nil`.                           |
| [`predicate-position?`](proofread.inspect.forms/predicate-position-p.html)               | Test whether an AST node is used in a boolean predicate position.       |
| [`same-form?`](proofread.inspect.forms/same-form-p.html)                                 | Test whether two AST nodes represent the same source form.              |
| [`single-binding-symbol`](proofread.inspect.forms/single-binding-symbol.html)            | Return the binding symbol for a single-binding `let` form.              |
| [`sole-let-body-form`](proofread.inspect.forms/sole-let-body-form.html)                  | Return the sole body form for a single-body `let`.                      |
| [`symbol-name`](proofread.inspect.forms/symbol-name.html)                                | Return the symbol name for an AST node.                                 |
| [`thread-first-call?`](proofread.inspect.forms/thread-first-call-p.html)                 | Test whether an AST node is a callable thread-first step.               |
| [`thread-first-form?`](proofread.inspect.forms/thread-first-form-p.html)                 | Test whether an AST node is a `->` form.                                |
| [`thread-first-transform?`](proofread.inspect.forms/thread-first-transform-p.html)       | Test whether a form threads `value` as its first argument.              |
| [`zero-node?`](proofread.inspect.forms/zero-node-p.html)                                 | Test whether an AST node is the number `0`.                             |

</main>
</div>
