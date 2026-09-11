---
layout: reference
title: loom.package.preflight/unsafe-package-path?
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.package.preflight.html">loom.package.preflight</a>
<ol>
<li><a href="check.html">check</a></li>
<li><a href="collect-path-diagnostic.html">collect-path-diagnostic</a></li>
<li><a href="load-root-diagnostics.html">load-root-diagnostics</a></li>
<li><a href="native-library-diagnostics.html">native-library-diagnostics</a></li>
<li><a href="native-library-source-diagnostics.html">native-library-source-diagnostics</a></li>
<li><a href="payload-path-diagnostic.html">payload-path-diagnostic</a></li>
<li><a class="active" href="unsafe-package-path-p.html">unsafe-package-path?</a></li>
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
<span aria-current="page">unsafe-package-path?</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.package.preflight/unsafe-package-path-p.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.package.preflight.html">loom.package.preflight</a>/</div>
<h1><code>unsafe-package-path?</code></h1>
</header>

Determine whether PATH may escape or ambiguously address the package directory.

<hr class="signature-divider">

```roo
(unsafe-package-path? path)
```

<hr class="signature-divider">

## Signature:

```roo
(unsafe-package-path? path)
```

| Arg  | Description                                                                       |
| ---- | --------------------------------------------------------------------------------- |
| path | Package payload path to inspect. Backslashes are normalized before<br>validation. |

### Returns:

True when PATH is blank, absolute, drive-qualified, or contains a parent-directory
segment; otherwise false.

</main>
</div>
