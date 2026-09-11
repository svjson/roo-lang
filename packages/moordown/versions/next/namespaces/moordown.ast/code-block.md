---
layout: reference
title: moordown.ast/code-block
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../moordown.ast.html">moordown.ast</a>
<ol>
<li><a href="code.html">code</a></li>
<li><a class="active" href="code-block.html">code-block</a></li>
<li><a href="document.html">document</a></li>
<li><a href="heading.html">heading</a></li>
<li><a href="link.html">link</a></li>
<li><a href="markdown-text.html">markdown-text</a></li>
<li><a href="paragraph.html">paragraph</a></li>
<li><a href="table.html">table</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">moordown next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../moordown.ast.html">moordown.ast</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">code-block</span>
</nav>
<div class="version-context">
<span class="version-package">moordown</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["moordown"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/moordown.ast/code-block.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../moordown.ast.html">moordown.ast</a>/</div>
<h1><code>code-block</code></h1>
</header>

Create a fenced Markdown code block node.

<hr class="signature-divider">

```roo
(code-block language code)
```

<hr class="signature-divider">

## Signature:

```roo
(code-block language code)
```

| Arg      | Description                                          |
| -------- | ---------------------------------------------------- |
| language | Fence language label, or nil for an unlabeled fence. |
| code     | Code text.                                           |

### Returns:

A `:code-block` node.

</main>
</div>
