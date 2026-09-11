---
layout: reference
title: proofread.inspect.forms/call?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.inspect.forms.html">proofread.inspect.forms</a>
<ol>
<li><a href="branch-predicate-position-p.html">branch-predicate-position?</a></li>
<li><a class="active" href="call-p.html">call?</a></li>
<li><a href="cond-test-child-p.html">cond-test-child?</a></li>
<li><a href="count-argument.html">count-argument</a></li>
<li><a href="count-call-p.html">count-call?</a></li>
<li><a href="direct-symbol-p.html">direct-symbol?</a></li>
<li><a href="else-child-p.html">else-child?</a></li>
<li><a href="empty-fallback-p.html">empty-fallback?</a></li>
<li><a href="empty-map-p.html">empty-map?</a></li>
<li><a href="empty-vector-p.html">empty-vector?</a></li>
<li><a href="fn-single-param-name.html">fn-single-param-name</a></li>
<li><a href="form-sequence-string.html">form-sequence-string</a></li>
<li><a href="form-string.html">form-string</a></li>
<li><a href="full-if-p.html">full-if?</a></li>
<li><a href="head-name.html">head-name</a></li>
<li><a href="if-else.html">if-else</a></li>
<li><a href="keyword-lookup-key-name.html">keyword-lookup-key-name</a></li>
<li><a href="keyword-lookup-p.html">keyword-lookup?</a></li>
<li><a href="keyword-name.html">keyword-name</a></li>
<li><a href="literal-number-p.html">literal-number?</a></li>
<li><a href="literal-string-with-len-p.html">literal-string-with-len?</a></li>
<li><a href="literal-string-p.html">literal-string?</a></li>
<li><a href="map-form-p.html">map-form?</a></li>
<li><a href="nil-node-p.html">nil-node?</a></li>
<li><a href="predicate-position-p.html">predicate-position?</a></li>
<li><a href="same-form-p.html">same-form?</a></li>
<li><a href="single-binding-symbol.html">single-binding-symbol</a></li>
<li><a href="sole-let-body-form.html">sole-let-body-form</a></li>
<li><a href="symbol-name.html">symbol-name</a></li>
<li><a href="thread-first-call-p.html">thread-first-call?</a></li>
<li><a href="thread-first-form-p.html">thread-first-form?</a></li>
<li><a href="thread-first-transform-p.html">thread-first-transform?</a></li>
<li><a href="zero-node-p.html">zero-node?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.inspect.forms.html">proofread.inspect.forms</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">call?</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.inspect.forms/call-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.inspect.forms.html">proofread.inspect.forms</a>/</div>
<h1><code>call?</code></h1>
</header>

Test whether an AST node is a list call with a specific head and arity.

<hr class="signature-divider">

```roo
(call? {:keys [children] :as node} head arity)
```

<hr class="signature-divider">

## Signature:

```roo
(call? {:keys [children] :as node} head arity)
```

| Arg                         | Description                                     |
| --------------------------- | ----------------------------------------------- |
| {:keys [children] :as node} |                                                 |
| head                        | Expected call head name.                        |
| arity                       | Expected child count including the head symbol. |

### Returns:

`true` when `node` is a list headed by `head` with `arity` children,
otherwise `false`.

- `node`: A `roo.ast` node map.

### Returns:

`true` when `node` is a list headed by `head` with `arity` children,
otherwise `false`.

</main>
</div>
