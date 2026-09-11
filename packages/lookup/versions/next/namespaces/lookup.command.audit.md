---
layout: reference
title: lookup.command.audit
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="lookup.command.audit.html">lookup.command.audit</a>
<ol>
<li><a href="lookup.command.audit/audit-index-bang.html">audit-index!</a></li>
<li><a href="lookup.command.audit/command.html">command</a></li>
<li><a href="lookup.command.audit/diagnostic-extra.html">diagnostic-extra</a></li>
<li><a href="lookup.command.audit/diagnostic-line.html">diagnostic-line</a></li>
<li><a href="lookup.command.audit/execute-bang.html">execute!</a></li>
<li><a href="lookup.command.audit/text-lines.html">text-lines</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">lookup next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">lookup.command.audit</span>
</nav>
<div class="version-context">
<span class="version-package">lookup</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["lookup"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/lookup.command.audit.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# lookup.command.audit

| Symbol                                                           | Summary                                                                  |
| ---------------------------------------------------------------- | ------------------------------------------------------------------------ |
| [`audit-index!`](lookup.command.audit/audit-index-bang.html)     | Load or build the symbol index selected by audit options.                |
| [`command`](lookup.command.audit/command.html)                   | cli-trooper declaration for the `lookup audit` command.                  |
| [`diagnostic-extra`](lookup.command.audit/diagnostic-extra.html) | Format optional signature and parameter context for an audit diagnostic. |
| [`diagnostic-line`](lookup.command.audit/diagnostic-line.html)   | Render one audit diagnostic as a text line.                              |
| [`execute!`](lookup.command.audit/execute-bang.html)             | Audit the selected index or source input and emit the requested format.  |
| [`text-lines`](lookup.command.audit/text-lines.html)             | Render an audit result as text output.                                   |

</main>
</div>
