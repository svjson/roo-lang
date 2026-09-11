---
layout: reference
title: loom.package.preflight/collect-path-diagnostic
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.package.preflight.html">loom.package.preflight</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a class="active" href="collect-path-diagnostic.html">collect-path-diagnostic</a></li>
<li><a href="load-root-diagnostics.html">load-root-diagnostics</a></li>
<li><a href="native-library-diagnostics.html">native-library-diagnostics</a></li>
<li><a href="native-library-source-diagnostics.html">native-library-source-diagnostics</a></li>
<li><a href="payload-path-diagnostic.html">payload-path-diagnostic</a></li>
<li><a href="unsafe-package-path-p.html">unsafe-package-path?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.package.preflight.html">loom.package.preflight</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">collect-path-diagnostic</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.package.preflight/collect-path-diagnostic.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.package.preflight.html">loom.package.preflight</a>/</div>
<h1><code>collect-path-diagnostic</code></h1>
</header>

Append a present path diagnostic to a diagnostic collection.

<hr class="signature-divider">

```roo
(collect-path-diagnostic diagnostics diagnostic)
```

<hr class="signature-divider">

## Signature:

```roo
(collect-path-diagnostic diagnostics diagnostic)
```

| Arg         | Description                                                 |
| ----------- | ----------------------------------------------------------- |
| diagnostics | Existing diagnostic collection.                             |
| diagnostic  | Diagnostic map to append, or nil when validation succeeded. |

### Returns:

DIAGNOSTICS unchanged when DIAGNOSTIC is nil; otherwise DIAGNOSTICS with
DIAGNOSTIC appended.

</main>
</div>
