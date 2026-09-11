---
layout: reference
title: voodoo.editor/compose
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../voodoo.editor.html">voodoo.editor</a>
<ol>
<li><a class="active" href="compose.html">compose</a></li>
<li><a href="dispatch.html">dispatch</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../voodoo.editor.html">voodoo.editor</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">compose</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/voodoo.editor/compose.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../voodoo.editor.html">voodoo.editor</a>/</div>
<h1><code>compose</code></h1>
</header>

Compose EDITORS from general behavior to increasingly specific overlays.

<hr class="signature-divider">

```roo
(compose editor &rest)
```

<hr class="signature-divider">

## Signature:

```roo
(compose editor &rest)
```

| Arg    | Description                                                                                                         |
| ------ | ------------------------------------------------------------------------------------------------------------------- |
| editor | Base editor map containing an ordered `:layers` vector.                                                             |
| &rest  | Zero or more overlay editor maps containing ordered `:layers`<br>vectors, collected by Roo's rest-argument binding. |

### Returns:

An editor retaining top-level configuration from the supplied definitions
and shaped with `{:layers [base-layer ... overlay-layer]}`. Layers preserve
base-to-overlay order, while later top-level values replace earlier ones.
Later layers receive the first opportunity to handle an event.

</main>
</div>
