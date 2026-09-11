---
layout: reference
title: roopl.repl
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roopl.repl.html">roopl.repl</a>
<ol>
<li><a href="roopl.repl/close-bang.html">close!</a></li>
<li><a href="roopl.repl/current-namespace.html">current-namespace</a></li>
<li><a href="roopl.repl/make.html">make</a></li>
<li><a href="roopl.repl/submit-bang.html">submit!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roopl next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roopl.repl</span>
</nav>
<div class="version-context">
<span class="version-package">roopl</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roopl"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roopl.repl.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roopl.repl

| Symbol                                                   | Summary                                                          |
| -------------------------------------------------------- | ---------------------------------------------------------------- |
| [`close!`](roopl.repl/close-bang.html)                   | Release the evaluation target owned by REPL.                     |
| [`current-namespace`](roopl.repl/current-namespace.html) | Return the namespace used for the next submission.               |
| [`make`](roopl.repl/make.html)                           | Create REPL state for TARGET.                                    |
| [`submit!`](roopl.repl/submit-bang.html)                 | Evaluate SOURCE and advance REPL submission and namespace state. |

</main>
</div>
