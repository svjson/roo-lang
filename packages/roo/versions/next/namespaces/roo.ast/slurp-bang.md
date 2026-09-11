---
layout: reference
title: roo.ast/slurp!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.ast.html">roo.ast</a>
<ol>
<li><a href="read-string.html">read-string</a></li>
<li><a class="active" href="slurp-bang.html">slurp!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.ast.html">roo.ast</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">slurp!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.ast/slurp-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.ast.html">roo.ast</a>/</div>
<h1><code>slurp!</code></h1>
</header>

Read a Roo source file into top-level AST node entries.

<hr class="signature-divider">

```roo
(slurp! file-path)
```

<hr class="signature-divider">

### Usage:

```roo
(roo.ast/slurp! "src/app/core.roo")
=> [{:type :list :form (ns app.core) :source {...} :children [...]}]
```

Each entry contains the parsed Roo form as Roo data, its AST node type,
source locations with file path, line, column, start, and end positions, and
nested child nodes for sequence forms.

<hr class="signature-divider">

## Signature:

```roo
(slurp! file-path)
```

| Arg       | Description                          |
| --------- | ------------------------------------ |
| file-path | The path of the source file to read. |

</main>
</div>
