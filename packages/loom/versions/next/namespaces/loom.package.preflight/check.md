---
layout: reference
title: loom.package.preflight/check
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.package.preflight.html">loom.package.preflight</a>
<ol>
<li><a class="active" href="check.html">check</a></li>
<li><a href="collect-path-diagnostic.html">collect-path-diagnostic</a></li>
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
<span aria-current="page">check</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.package.preflight/check.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.package.preflight.html">loom.package.preflight</a>/</div>
<h1><code>check</code></h1>
</header>

Check whether a package directory contains a complete installable artifact.

<hr class="signature-divider">

```roo
(check package-dir)
```

<hr class="signature-divider">

## Signature:

```roo
(check package-dir)
```

| Arg         | Description                                                                  |
| ----------- | ---------------------------------------------------------------------------- |
| package-dir | Root directory containing the package manifest and all declared<br>payloads. |

### Returns:

A result map containing `:ok?` and `:diagnostics`. Missing or invalid manifests
return `:ok? false` with manifest diagnostics. Invalid or missing payloads also
return the validated `:manifest`. A successful result returns `:ok? true`, the
validated `:manifest`, a serialized symbol index in `:symbols`, and an empty
diagnostic vector.

</main>
</div>
