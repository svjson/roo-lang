---
layout: reference
title: cli-trooper.parse.default/resolve
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.parse.default.html">cli-trooper.parse.default</a>
<ol>
<li><a class="active" href="resolve.html">resolve</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.parse.default.html">cli-trooper.parse.default</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">resolve</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper.parse.default/resolve.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.parse.default.html">cli-trooper.parse.default</a>/</div>
<h1><code>resolve</code></h1>
</header>

Resolve pending defaults into canonical command option order.

<hr class="signature-divider">

```roo
(resolve spec options defaults supplied)
```

<hr class="signature-divider">

## Signature:

```roo
(resolve spec options defaults supplied)
```

| Arg      | Description                                                                                                                                                                                                 |
| -------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| spec     | The selected canonical command specification.                                                                                                                                                               |
| options  | Values successfully captured from the argument sequence.                                                                                                                                                    |
| defaults | Pending declaration defaults and invocation options whose<br>destinations were not claimed by explicit arguments. Declaration resolver<br>functions receive the captured options and every pending literal. |
| supplied | Destinations whose pending values came from invocation options<br>rather than the canonical declaration.                                                                                                    |

### Returns:

A flat options map containing captured values and resolved defaults in
canonical declaration order. A claimed destination without a captured value
is omitted.

</main>
</div>
