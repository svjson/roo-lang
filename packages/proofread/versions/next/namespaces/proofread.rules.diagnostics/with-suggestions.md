---
layout: reference
title: proofread.rules.diagnostics/with-suggestions
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>
<ol>
<li><a href="edit.html">edit</a></li>
<li><a href="make.html">make</a></li>
<li><a href="make-file.html">make-file</a></li>
<li><a href="make-suggesting.html">make-suggesting</a></li>
<li><a href="rewrite.html">rewrite</a></li>
<li><a class="active" href="with-suggestions.html">with-suggestions</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">with-suggestions</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.rules.diagnostics/with-suggestions.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.rules.diagnostics.html">proofread.rules.diagnostics</a>/</div>
<h1><code>with-suggestions</code></h1>
</header>

Attach suggestions to a diagnostic.

<hr class="signature-divider">

```roo
(with-suggestions diagnostic suggestions)
```

<hr class="signature-divider">

## Signature:

```roo
(with-suggestions diagnostic suggestions)
```

| Arg         | Description      |
| ----------- | ---------------- |
| diagnostic  | Diagnostic map.  |
| suggestions | Suggestion maps. |

### Returns:

`diagnostic` with `:suggestions` when suggestions are present.

</main>
</div>
