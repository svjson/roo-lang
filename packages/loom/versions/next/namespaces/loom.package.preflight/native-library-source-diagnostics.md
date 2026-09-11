---
layout: reference
title: loom.package.preflight/native-library-source-diagnostics
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.package.preflight.html">loom.package.preflight</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="collect-path-diagnostic.html">collect-path-diagnostic</a></li>
<li><a href="load-root-diagnostics.html">load-root-diagnostics</a></li>
<li><a href="native-library-diagnostics.html">native-library-diagnostics</a></li>
<li><a class="active" href="native-library-source-diagnostics.html">native-library-source-diagnostics</a></li>
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
<span aria-current="page">native-library-source-diagnostics</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.package.preflight/native-library-source-diagnostics.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.package.preflight.html">loom.package.preflight</a>/</div>
<h1><code>native-library-source-diagnostics</code></h1>
</header>

Validate a native-library entry's declared `:source` roots, if any.

<hr class="signature-divider">

```roo
(native-library-source-diagnostics package-dir library)
```

<hr class="signature-divider">

`:source` names where a native library's actual source lives when that
differs from `:path` (the compiled-artifact location `loom install`
copies) - see `lookup.source.native.parse.entries/native-library-source-roots`.
A missing or invalid `:source` root fails preflight loudly rather than
silently producing an empty native documentation index, the failure mode
that motivated adding this validation in the first place.

<hr class="signature-divider">

## Signature:

```roo
(native-library-source-diagnostics package-dir library)
```

| Arg         | Description                                  |
| ----------- | -------------------------------------------- |
| package-dir | Root directory of the package being checked. |
| library     | One `:native-libraries` entry map.           |

### Returns:

A vector of diagnostics for invalid, unsafe, missing, or non-directory
source roots. The vector is empty when `:source` is absent or every
declared root is valid.

</main>
</div>
