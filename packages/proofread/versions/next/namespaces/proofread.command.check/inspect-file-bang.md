---
layout: reference
title: proofread.command.check/inspect-file!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.command.check.html">proofread.command.check</a>
<ol>
<li><a href="await-execution-bang.html">await-execution!</a></li>
<li><a href="collect-files-bang.html">collect-files!</a></li>
<li><a href="command.html">command</a></li>
<li><a href="diagnostic-count.html">diagnostic-count</a></li>
<li><a href="execute-bang.html">execute!</a></li>
<li><a class="active" href="inspect-file-bang.html">inspect-file!</a></li>
<li><a href="record-file-result-bang.html">record-file-result!</a></li>
<li><a href="validation-errors.html">validation-errors</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.command.check.html">proofread.command.check</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">inspect-file!</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.command.check/inspect-file-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.command.check.html">proofread.command.check</a>/</div>
<h1><code>inspect-file!</code></h1>
</header>

Inspect one file on the application worker.

<hr class="signature-divider">

```roo
(inspect-file! widget-session file input-options)
```

<hr class="signature-divider">

## Signature:

```roo
(inspect-file! widget-session file input-options)
```

| Arg            | Description                                          |
| -------------- | ---------------------------------------------------- |
| widget-session | Voodoo session owned by the parent runtime.          |
| file           | Filesystem path or logical standard-input filename.  |
| input-options  | Inspection options, optionally containing `:source`. |

### Returns:

A transferable `proofread.inspection/file-result` map. Worker failures
terminate the command with a controlled diagnostic.

</main>
</div>
