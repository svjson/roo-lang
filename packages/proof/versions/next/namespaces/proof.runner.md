---
layout: reference
title: proof.runner
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.runner.html">proof.runner</a>
<ol>
<li><a href="proof.runner/absolute-fixture-root.html">absolute-fixture-root</a></li>
<li><a href="proof.runner/absolute-test-path.html">absolute-test-path</a></li>
<li><a href="proof.runner/discover-roo-files.html">discover-roo-files</a></li>
<li><a href="proof.runner/discover-test-files.html">discover-test-files</a></li>
<li><a href="proof.runner/ensure-test-file-entry.html">ensure-test-file-entry</a></li>
<li><a href="proof.runner/execute-bang.html">execute!</a></li>
<li><a href="proof.runner/load-tests.html">load-tests</a></li>
<li><a href="proof.runner/normalized-path.html">normalized-path</a></li>
<li><a href="proof.runner/relative-package-path.html">relative-package-path</a></li>
<li><a href="proof.runner/require-form.html">require-form</a></li>
<li><a href="proof.runner/roo-file-entry-p.html">roo-file-entry?</a></li>
<li><a href="proof.runner/run.html">run</a></li>
<li><a href="proof.runner/trim-trailing-slash.html">trim-trailing-slash</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.runner</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.runner.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.runner

| Symbol                                                               | Summary                                                                  |
| -------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| [`absolute-fixture-root`](proof.runner/absolute-fixture-root.html)   | Resolve a configured fixture root against the invoking package root.     |
| [`absolute-test-path`](proof.runner/absolute-test-path.html)         | Resolve one configured test path against the invoking package root.      |
| [`discover-roo-files`](proof.runner/discover-roo-files.html)         | Discover Roo source files beneath one test root.                         |
| [`discover-test-files`](proof.runner/discover-test-files.html)       | Discover Roo files from every configured test path.                      |
| [`ensure-test-file-entry`](proof.runner/ensure-test-file-entry.html) | Append metadata for a test file unless its namespace is already present. |
| [`execute!`](proof.runner/execute-bang.html)                         | Execute a parsed Proof command and interpret its test outcome as status. |
| [`load-tests`](proof.runner/load-tests.html)                         | Clear the registry, load discovered test namespaces, and annotate tests. |
| [`normalized-path`](proof.runner/normalized-path.html)               | Normalize path separators for display and prefix comparison.             |
| [`relative-package-path`](proof.runner/relative-package-path.html)   | Build a stable package-relative display path.                            |
| [`require-form`](proof.runner/require-form.html)                     | Build the namespace form used to load discovered test namespaces.        |
| [`roo-file-entry?`](proof.runner/roo-file-entry-p.html)              | Determine whether a walked filesystem entry is a Roo source file.        |
| [`run`](proof.runner/run.html)                                       | Discover and run tests from an already coherent Proof configuration.     |
| [`trim-trailing-slash`](proof.runner/trim-trailing-slash.html)       | Remove a non-root path's trailing slash.                                 |

</main>
</div>
