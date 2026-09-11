---
layout: reference
title: proof.tool/run!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.tool.html">proof.tool</a>
<ol>
<li><a class="active" href="run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.tool.html">proof.tool</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">run!</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.tool/run-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.tool.html">proof.tool</a>/</div>
<h1><code>run!</code></h1>
</header>

Run Proof through the Roo package-tool boundary.

<hr class="signature-divider">

```roo
(run! {:keys [config] :as context})
```

<hr class="signature-divider">

## Signature:

```roo
(run! {:keys [config] :as context})
```

| Arg                          | Description |
| ---------------------------- | ----------- |
| {:keys [config] :as context} |             |

### Returns:

Process status `0` for a passing run or help, `1` for a failing or errored
test run, or `2` for invalid command-line input.

- `context`: Package-tool context containing command-line arguments under
`:args`, package-relative execution data such as `:package-root`, and the
selected package's Proof configuration under `:config`.

### Returns:

Process status `0` for a passing run or help, `1` for a failing or errored
test run, or `2` for invalid command-line input.

</main>
</div>
