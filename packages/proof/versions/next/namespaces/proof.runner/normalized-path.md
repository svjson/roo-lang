---
layout: reference
title: proof.runner/normalized-path
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.runner.html">proof.runner</a>
<ol>
<li><a href="absolute-fixture-root.html">absolute-fixture-root</a></li>
<li><a href="absolute-test-path.html">absolute-test-path</a></li>
<li><a href="discover-roo-files.html">discover-roo-files</a></li>
<li><a href="discover-test-files.html">discover-test-files</a></li>
<li><a href="ensure-test-file-entry.html">ensure-test-file-entry</a></li>
<li><a href="execute-bang.html">execute!</a></li>
<li><a href="load-tests.html">load-tests</a></li>
<li><a class="active" href="normalized-path.html">normalized-path</a></li>
<li><a href="relative-package-path.html">relative-package-path</a></li>
<li><a href="require-form.html">require-form</a></li>
<li><a href="roo-file-entry-p.html">roo-file-entry?</a></li>
<li><a href="run.html">run</a></li>
<li><a href="trim-trailing-slash.html">trim-trailing-slash</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.runner.html">proof.runner</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">normalized-path</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.runner/normalized-path.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.runner.html">proof.runner</a>/</div>
<h1><code>normalized-path</code></h1>
</header>

Normalize path separators for display and prefix comparison.

<hr class="signature-divider">

```roo
(normalized-path path)
```

<hr class="signature-divider">

## Signature:

```roo
(normalized-path path)
```

| Arg  | Description            |
| ---- | ---------------------- |
| path | Path-compatible value. |

### Returns:

String path with backslashes replaced by forward slashes.

</main>
</div>
