---
layout: reference
title: proofread.reporter.common
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.reporter.common.html">proofread.reporter.common</a>
<ol>
<li><a href="proofread.reporter.common/diagnostic-line.html">diagnostic-line</a></li>
<li><a href="proofread.reporter.common/diagnostic-line-with-options.html">diagnostic-line-with-options</a></li>
<li><a href="proofread.reporter.common/diagnostic-location.html">diagnostic-location</a></li>
<li><a href="proofread.reporter.common/diagnostic-location-with-options.html">diagnostic-location-with-options</a></li>
<li><a href="proofread.reporter.common/error-styles.html">error-styles</a></li>
<li><a href="proofread.reporter.common/filename-styles.html">filename-styles</a></li>
<li><a href="proofread.reporter.common/group-criterion-styles.html">group-criterion-styles</a></li>
<li><a href="proofread.reporter.common/group-line.html">group-line</a></li>
<li><a href="proofread.reporter.common/group-value-styles.html">group-value-styles</a></li>
<li><a href="proofread.reporter.common/location-position-styles.html">location-position-styles</a></li>
<li><a href="proofread.reporter.common/location-prefix.html">location-prefix</a></li>
<li><a href="proofread.reporter.common/location-separator-styles.html">location-separator-styles</a></li>
<li><a href="proofread.reporter.common/print-summary-bang.html">print-summary!</a></li>
<li><a href="proofread.reporter.common/rule-styles.html">rule-styles</a></li>
<li><a href="proofread.reporter.common/severity-styles.html">severity-styles</a></li>
<li><a href="proofread.reporter.common/success-styles.html">success-styles</a></li>
<li><a href="proofread.reporter.common/summary-styles.html">summary-styles</a></li>
<li><a href="proofread.reporter.common/summary-text.html">summary-text</a></li>
<li><a href="proofread.reporter.common/warning-styles.html">warning-styles</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.reporter.common</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.reporter.common.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.reporter.common

| Symbol                                                                                                | Summary                                                            |
| ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`diagnostic-line`](proofread.reporter.common/diagnostic-line.html)                                   | Format one styled diagnostic for CLI output.                       |
| [`diagnostic-line-with-options`](proofread.reporter.common/diagnostic-line-with-options.html)         | Format one styled diagnostic for CLI output with display options.  |
| [`diagnostic-location`](proofread.reporter.common/diagnostic-location.html)                           | Format one styled diagnostic source location.                      |
| [`diagnostic-location-with-options`](proofread.reporter.common/diagnostic-location-with-options.html) | Format one styled diagnostic source location with display options. |
| [`error-styles`](proofread.reporter.common/error-styles.html)                                         | Styles used for error output.                                      |
| [`filename-styles`](proofread.reporter.common/filename-styles.html)                                   | Styles used for diagnostic source filenames.                       |
| [`group-criterion-styles`](proofread.reporter.common/group-criterion-styles.html)                     | Styles used for grouped reporter criterion labels.                 |
| [`group-line`](proofread.reporter.common/group-line.html)                                             | Format one grouped reporter heading.                               |
| [`group-value-styles`](proofread.reporter.common/group-value-styles.html)                             | Styles used for grouped reporter values.                           |
| [`location-position-styles`](proofread.reporter.common/location-position-styles.html)                 | Styles used for diagnostic line and column numbers.                |
| [`location-prefix`](proofread.reporter.common/location-prefix.html)                                   | Return the directory prefix for a diagnostic path.                 |
| [`location-separator-styles`](proofread.reporter.common/location-separator-styles.html)               | Styles used for diagnostic location separators.                    |
| [`print-summary!`](proofread.reporter.common/print-summary-bang.html)                                 | Print a proofread run summary.                                     |
| [`rule-styles`](proofread.reporter.common/rule-styles.html)                                           | Styles used for diagnostic rule names.                             |
| [`severity-styles`](proofread.reporter.common/severity-styles.html)                                   | Return styles for a diagnostic severity.                           |
| [`success-styles`](proofread.reporter.common/success-styles.html)                                     | Styles used for successful summary output.                         |
| [`summary-styles`](proofread.reporter.common/summary-styles.html)                                     | Return styles for a proofread summary line.                        |
| [`summary-text`](proofread.reporter.common/summary-text.html)                                         | Format a proofread run summary.                                    |
| [`warning-styles`](proofread.reporter.common/warning-styles.html)                                     | Styles used for warning output.                                    |

</main>
</div>
