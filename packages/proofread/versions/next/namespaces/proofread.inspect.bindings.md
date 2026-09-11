---
layout: reference
title: proofread.inspect.bindings
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.inspect.bindings.html">proofread.inspect.bindings</a>
<ol>
<li><a href="proofread.inspect.bindings/binding-use-count.html">binding-use-count</a></li>
<li><a href="proofread.inspect.bindings/body-use-count.html">body-use-count</a></li>
<li><a href="proofread.inspect.bindings/first-single-use-simple-binding.html">first-single-use-simple-binding</a></li>
<li><a href="proofread.inspect.bindings/later-binding-use-count.html">later-binding-use-count</a></li>
<li><a href="proofread.inspect.bindings/let-bindings.html">let-bindings</a></li>
<li><a href="proofread.inspect.bindings/let-body-forms.html">let-body-forms</a></li>
<li><a href="proofread.inspect.bindings/single-use-simple-binding-p.html">single-use-simple-binding?</a></li>
<li><a href="proofread.inspect.bindings/symbol-use-count.html">symbol-use-count</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.inspect.bindings</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.inspect.bindings.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.inspect.bindings

| Symbol                                                                                               | Summary                                                  |
| ---------------------------------------------------------------------------------------------------- | -------------------------------------------------------- |
| [`binding-use-count`](proofread.inspect.bindings/binding-use-count.html)                             | Count uses of one let binding in its scope.              |
| [`body-use-count`](proofread.inspect.bindings/body-use-count.html)                                   | Count symbol uses in body forms.                         |
| [`first-single-use-simple-binding`](proofread.inspect.bindings/first-single-use-simple-binding.html) | Return the first single-use simple binding symbol node.  |
| [`later-binding-use-count`](proofread.inspect.bindings/later-binding-use-count.html)                 | Count symbol uses in later binding values.               |
| [`let-bindings`](proofread.inspect.bindings/let-bindings.html)                                       | Return binding children for a `let` form.                |
| [`let-body-forms`](proofread.inspect.bindings/let-body-forms.html)                                   | Return body children for a `let` form.                   |
| [`single-use-simple-binding?`](proofread.inspect.bindings/single-use-simple-binding-p.html)          | Test whether one binding has a simple value and one use. |
| [`symbol-use-count`](proofread.inspect.bindings/symbol-use-count.html)                               | Count direct symbol uses in an AST subtree.              |

</main>
</div>
