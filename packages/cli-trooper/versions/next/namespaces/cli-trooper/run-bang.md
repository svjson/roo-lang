---
layout: reference
title: cli-trooper/run!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../cli-trooper.html">cli-trooper</a>
<ol>
<li><a href="parse.html">parse</a></li>
<li><a href="prepare.html">prepare</a></li>
<li><a href="run.html">run</a></li>
<li><a class="active" href="run-bang.html">run!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">cli-trooper next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../cli-trooper.html">cli-trooper</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">run!</span>
</nav>
<div class="version-context">
<span class="version-package">cli-trooper</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["cli-trooper"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/cli-trooper/run-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../cli-trooper.html">cli-trooper</a>/</div>
<h1><code>run!</code></h1>
</header>

Run a declared CLI command and return a process-compatible exit code.

<hr class="signature-divider">

```roo
(run! supplied context)
```

<hr class="signature-divider">

Structured invocation diagnostics are rendered and printed when parsing or
dispatch fails. Ordinary command output and result interpretation remain
application-owned.

<hr class="signature-divider">

## Signature:

```roo
(run! supplied context)
```

| Arg      | Description                                                                                                       |
| -------- | ----------------------------------------------------------------------------------------------------------------- |
| supplied | The supplied cli-trooper application configuration.                                                               |
| context  | The CLI invocation context containing arguments under `:args`<br>and optional canonical options under `:options`. |

### Returns:

The integer returned by the command, `0` after a successful command returns
any other value, or `2` after a configuration, argument, conversion,
validation, or execution-resolution failure.

</main>
</div>
