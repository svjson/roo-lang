---
layout: reference
title: loom.command.install/install!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../loom.command.install.html">loom.command.install</a>
<ol>
<li><a href="copy-load-roots-bang.html">copy-load-roots!</a></li>
<li><a href="copy-native-libraries-bang.html">copy-native-libraries!</a></li>
<li><a href="copy-native-library-bang.html">copy-native-library!</a></li>
<li><a class="active" href="install-bang.html">install!</a></li>
<li><a href="report-preflight-failure-bang.html">report-preflight-failure!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../loom.command.install.html">loom.command.install</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">install!</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/loom.command.install/install-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../loom.command.install.html">loom.command.install</a>/</div>
<h1><code>install!</code></h1>
</header>

<hr class="signature-divider">

```roo
(install! package-dir repo-root force?)
```

<hr class="signature-divider">

## Signature:

```roo
(install! package-dir repo-root force?)
```

| Arg         | Description |
| ----------- | ----------- |
| package-dir |             |
| repo-root   |             |
| force?      |             |

</main>
</div>
