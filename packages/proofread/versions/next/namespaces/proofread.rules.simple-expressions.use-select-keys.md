---
layout: reference
title: proofread.rules.simple-expressions.use-select-keys
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.rules.simple-expressions.use-select-keys.html">proofread.rules.simple-expressions.use-select-keys</a>
<ol>
<li><a href="proofread.rules.simple-expressions.use-select-keys/check.html">check</a></li>
<li><a href="proofread.rules.simple-expressions.use-select-keys/select-keys-candidate-p.html">select-keys-candidate?</a></li>
<li><a href="proofread.rules.simple-expressions.use-select-keys/select-keys-entry-target.html">select-keys-entry-target</a></li>
<li><a href="proofread.rules.simple-expressions.use-select-keys/select-keys-target.html">select-keys-target</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.rules.simple-expressions.use-select-keys</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.rules.simple-expressions.use-select-keys.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.rules.simple-expressions.use-select-keys

| Symbol                                                                                                         | Summary                                                            |
| -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`check`](proofread.rules.simple-expressions.use-select-keys/check.html)                                       | Report map literals that should use `select-keys`.                 |
| [`select-keys-candidate?`](proofread.rules.simple-expressions.use-select-keys/select-keys-candidate-p.html)    | Test whether a map literal should be `select-keys`.                |
| [`select-keys-entry-target`](proofread.rules.simple-expressions.use-select-keys/select-keys-entry-target.html) | Return the source symbol name for a select-keys map entry.         |
| [`select-keys-target`](proofread.rules.simple-expressions.use-select-keys/select-keys-target.html)             | Return the shared source symbol for select-keys candidate entries. |

</main>
</div>
