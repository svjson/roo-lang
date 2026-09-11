---
layout: reference
title: loom.package.preflight
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="loom.package.preflight.html">loom.package.preflight</a>
<ol>
<li><a href="loom.package.preflight/check.html">check</a></li>
<li><a href="loom.package.preflight/collect-path-diagnostic.html">collect-path-diagnostic</a></li>
<li><a href="loom.package.preflight/load-root-diagnostics.html">load-root-diagnostics</a></li>
<li><a href="loom.package.preflight/native-library-diagnostics.html">native-library-diagnostics</a></li>
<li><a href="loom.package.preflight/native-library-source-diagnostics.html">native-library-source-diagnostics</a></li>
<li><a href="loom.package.preflight/payload-path-diagnostic.html">payload-path-diagnostic</a></li>
<li><a href="loom.package.preflight/unsafe-package-path-p.html">unsafe-package-path?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">loom.package.preflight</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/loom.package.preflight.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# loom.package.preflight

| Symbol                                                                                               | Summary                                                                         |
| ---------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| [`check`](loom.package.preflight/check.html)                                                         | Check whether a package directory contains a complete installable artifact.     |
| [`collect-path-diagnostic`](loom.package.preflight/collect-path-diagnostic.html)                     | Append a present path diagnostic to a diagnostic collection.                    |
| [`load-root-diagnostics`](loom.package.preflight/load-root-diagnostics.html)                         | Validate every load root declared by a package manifest.                        |
| [`native-library-diagnostics`](loom.package.preflight/native-library-diagnostics.html)               | Validate every native-library payload declared by a package manifest.           |
| [`native-library-source-diagnostics`](loom.package.preflight/native-library-source-diagnostics.html) | Validate a native-library entry's declared `:source` roots, if any.             |
| [`payload-path-diagnostic`](loom.package.preflight/payload-path-diagnostic.html)                     | Validate one declared package payload path.                                     |
| [`unsafe-package-path?`](loom.package.preflight/unsafe-package-path-p.html)                          | Determine whether PATH may escape or ambiguously address the package directory. |

</main>
</div>
