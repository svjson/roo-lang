---
layout: reference
title: lookup.source.native.parse.comment
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.source.native.parse.comment.html">lookup.source.native.parse.comment</a>
<ol>
<li><a href="lookup.source.native.parse.comment/annotation.html">annotation</a></li>
<li><a href="lookup.source.native.parse.comment/append-annotation-text.html">append-annotation-text</a></li>
<li><a href="lookup.source.native.parse.comment/append-header.html">append-header</a></li>
<li><a href="lookup.source.native.parse.comment/append-target-block.html">append-target-block</a></li>
<li><a href="lookup.source.native.parse.comment/arg-row.html">arg-row</a></li>
<li><a href="lookup.source.native.parse.comment/code-language.html">code-language</a></li>
<li><a href="lookup.source.native.parse.comment/directive-value.html">directive-value</a></li>
<li><a href="lookup.source.native.parse.comment/finish-block.html">finish-block</a></li>
<li><a href="lookup.source.native.parse.comment/finish-code.html">finish-code</a></li>
<li><a href="lookup.source.native.parse.comment/finish-text.html">finish-text</a></li>
<li><a href="lookup.source.native.parse.comment/header-p.html">header?</a></li>
<li><a href="lookup.source.native.parse.comment/identity.html">identity</a></li>
<li><a href="lookup.source.native.parse.comment/line-identity.html">line-identity</a></li>
<li><a href="lookup.source.native.parse.comment/lines.html">lines</a></li>
<li><a href="lookup.source.native.parse.comment/namespace-annotation.html">namespace-annotation</a></li>
<li><a href="lookup.source.native.parse.comment/param-directive.html">param-directive</a></li>
<li><a href="lookup.source.native.parse.comment/parse.html">parse</a></li>
<li><a href="lookup.source.native.parse.comment/return-text.html">return-text</a></li>
<li><a href="lookup.source.native.parse.comment/reuse-single-return.html">reuse-single-return</a></li>
<li><a href="lookup.source.native.parse.comment/start-args.html">start-args</a></li>
<li><a href="lookup.source.native.parse.comment/start-code.html">start-code</a></li>
<li><a href="lookup.source.native.parse.comment/start-param.html">start-param</a></li>
<li><a href="lookup.source.native.parse.comment/start-return.html">start-return</a></li>
<li><a href="lookup.source.native.parse.comment/step.html">step</a></li>
<li><a href="lookup.source.native.parse.comment/strip-first-prefix.html">strip-first-prefix</a></li>
<li><a href="lookup.source.native.parse.comment/strip-line.html">strip-line</a></li>
<li><a href="lookup.source.native.parse.comment/strip-suffix.html">strip-suffix</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.source.native.parse.comment</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.source.native.parse.comment.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.source.native.parse.comment

| Symbol                                                                                     | Summary                                                                  |
| ------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ |
| [`annotation`](lookup.source.native.parse.comment/annotation.html)                         | Parse an explicit Roo symbol annotation from a native block comment.     |
| [`append-annotation-text`](lookup.source.native.parse.comment/append-annotation-text.html) |                                                                          |
| [`append-header`](lookup.source.native.parse.comment/append-header.html)                   |                                                                          |
| [`append-target-block`](lookup.source.native.parse.comment/append-target-block.html)       |                                                                          |
| [`arg-row`](lookup.source.native.parse.comment/arg-row.html)                               |                                                                          |
| [`code-language`](lookup.source.native.parse.comment/code-language.html)                   |                                                                          |
| [`directive-value`](lookup.source.native.parse.comment/directive-value.html)               |                                                                          |
| [`finish-block`](lookup.source.native.parse.comment/finish-block.html)                     |                                                                          |
| [`finish-code`](lookup.source.native.parse.comment/finish-code.html)                       |                                                                          |
| [`finish-text`](lookup.source.native.parse.comment/finish-text.html)                       |                                                                          |
| [`header?`](lookup.source.native.parse.comment/header-p.html)                              |                                                                          |
| [`identity`](lookup.source.native.parse.comment/identity.html)                             | Parse `ClassName - roo.namespace/exported-symbol` from a native comment. |
| [`line-identity`](lookup.source.native.parse.comment/line-identity.html)                   |                                                                          |
| [`lines`](lookup.source.native.parse.comment/lines.html)                                   |                                                                          |
| [`namespace-annotation`](lookup.source.native.parse.comment/namespace-annotation.html)     | Parse an explicit Roo namespace annotation from a native block comment.  |
| [`param-directive`](lookup.source.native.parse.comment/param-directive.html)               |                                                                          |
| [`parse`](lookup.source.native.parse.comment/parse.html)                                   | Parse a Roo native C++ documentation comment.                            |
| [`return-text`](lookup.source.native.parse.comment/return-text.html)                       |                                                                          |
| [`reuse-single-return`](lookup.source.native.parse.comment/reuse-single-return.html)       |                                                                          |
| [`start-args`](lookup.source.native.parse.comment/start-args.html)                         |                                                                          |
| [`start-code`](lookup.source.native.parse.comment/start-code.html)                         |                                                                          |
| [`start-param`](lookup.source.native.parse.comment/start-param.html)                       |                                                                          |
| [`start-return`](lookup.source.native.parse.comment/start-return.html)                     |                                                                          |
| [`step`](lookup.source.native.parse.comment/step.html)                                     |                                                                          |
| [`strip-first-prefix`](lookup.source.native.parse.comment/strip-first-prefix.html)         |                                                                          |
| [`strip-line`](lookup.source.native.parse.comment/strip-line.html)                         |                                                                          |
| [`strip-suffix`](lookup.source.native.parse.comment/strip-suffix.html)                     |                                                                          |

</main>
</div>
