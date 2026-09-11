---
layout: reference
title: proofread.config/merge-config
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proofread.config.html">proofread.config</a>
<ol>
<li><a href="apply-diagnostic.html">apply-diagnostic</a></li>
<li><a href="apply-diagnostics.html">apply-diagnostics</a></li>
<li><a href="config-file.html">config-file</a></li>
<li><a href="config-files.html">config-files</a></li>
<li><a href="config-path.html">config-path</a></li>
<li><a href="diagnostic-severity.html">diagnostic-severity</a></li>
<li><a href="file-config.html">file-config</a></li>
<li><a class="active" href="merge-config.html">merge-config</a></li>
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
<span aria-current="page">merge-config</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proofread.config/merge-config.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proofread.config.html">proofread.config</a>/</div>
<h1><code>merge-config</code></h1>
</header>

Merge proofread config maps recursively.

<hr class="signature-divider">

```roo
(merge-config base overlay)
```

<hr class="signature-divider">

## Signature:

```roo
(merge-config base overlay)
```

| Arg     | Description        |
| ------- | ------------------ |
| base    | Parent config map. |
| overlay | Child config map.  |

### Returns:

A config map where child values override parent values. Maps merge
recursively; other values replace.

</main>
</div>
