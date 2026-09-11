---
layout: reference
title: proofread.reporter.grouped
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.reporter.grouped.html">proofread.reporter.grouped</a>
<ol>
<li><a href="proofread.reporter.grouped/append-group-value.html">append-group-value</a></li>
<li><a href="proofread.reporter.grouped/group-value.html">group-value</a></li>
<li><a href="proofread.reporter.grouped/group-values.html">group-values</a></li>
<li><a href="proofread.reporter.grouped/grouped-diagnostics.html">grouped-diagnostics</a></li>
<li><a href="proofread.reporter.grouped/indent.html">indent</a></li>
<li><a href="proofread.reporter.grouped/indent-width.html">indent-width</a></li>
<li><a href="proofread.reporter.grouped/lines.html">lines</a></li>
<li><a href="proofread.reporter.grouped/lines-at-level.html">lines-at-level</a></li>
<li><a href="proofread.reporter.grouped/lines-with-options.html">lines-with-options</a></li>
<li><a href="proofread.reporter.grouped/make.html">make</a></li>
<li><a href="proofread.reporter.grouped/report-file-bang.html">report-file!</a></li>
<li><a href="proofread.reporter.grouped/report-summary-bang.html">report-summary!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.reporter.grouped</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.reporter.grouped.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.reporter.grouped

| Symbol                                                                       | Summary                                                        |
| ---------------------------------------------------------------------------- | -------------------------------------------------------------- |
| [`append-group-value`](proofread.reporter.grouped/append-group-value.html)   | Append a diagnostic group value when it has not been seen.     |
| [`group-value`](proofread.reporter.grouped/group-value.html)                 | Return the grouping value for a diagnostic.                    |
| [`group-values`](proofread.reporter.grouped/group-values.html)               | Return distinct grouping values for diagnostics.               |
| [`grouped-diagnostics`](proofread.reporter.grouped/grouped-diagnostics.html) | Return diagnostics matching one group value.                   |
| [`indent`](proofread.reporter.grouped/indent.html)                           | Return indentation for a grouped reporter nesting level.       |
| [`indent-width`](proofread.reporter.grouped/indent-width.html)               | Number of spaces used for each grouped reporter nesting level. |
| [`lines`](proofread.reporter.grouped/lines.html)                             | Return grouped reporter diagnostic lines.                      |
| [`lines-at-level`](proofread.reporter.grouped/lines-at-level.html)           | Return grouped reporter lines for one nesting level.           |
| [`lines-with-options`](proofread.reporter.grouped/lines-with-options.html)   | Return grouped reporter diagnostic lines with display options. |
| [`make`](proofread.reporter.grouped/make.html)                               | Create a grouped proofread reporter.                           |
| [`report-file!`](proofread.reporter.grouped/report-file-bang.html)           | Collect diagnostics for grouped output.                        |
| [`report-summary!`](proofread.reporter.grouped/report-summary-bang.html)     | Print grouped diagnostics and the final proofread summary.     |

</main>
</div>
