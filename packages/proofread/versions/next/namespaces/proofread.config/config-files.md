---
layout: reference
title: proofread.config/config-files
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.config.html">proofread.config</a>
<ol>
<li><a href="apply-diagnostic.html">apply-diagnostic</a></li>
<li><a href="apply-diagnostics.html">apply-diagnostics</a></li>
<li><a href="config-file.html">config-file</a></li>
<li><a class="active" href="config-files.html">config-files</a></li>
<li><a href="config-path.html">config-path</a></li>
<li><a href="diagnostic-severity.html">diagnostic-severity</a></li>
<li><a href="file-config.html">file-config</a></li>
<li><a href="merge-config.html">merge-config</a></li>
<li><a href="merge-configs.html">merge-configs</a></li>
<li><a href="rule-enabled-p.html">rule-enabled?</a></li>
<li><a href="unknown-rules.html">unknown-rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proofread.config.html">proofread.config</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">config-files</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.config/config-files.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.config.html">proofread.config</a>/</div>
<h1><code>config-files</code></h1>
</header>

Find proofread config files that apply to a directory.

<hr class="signature-divider">

```roo
(config-files dir)
```

<hr class="signature-divider">

## Signature:

```roo
(config-files dir)
```

| Arg | Description                          |
| --- | ------------------------------------ |
| dir | Absolute or relative directory path. |

### Returns:

A vector of `.proofread.edn` paths ordered from parent to child.

</main>
</div>
