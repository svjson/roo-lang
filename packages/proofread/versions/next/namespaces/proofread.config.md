---
layout: reference
title: proofread.config
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proofread.config.html">proofread.config</a>
<ol>
<li><a href="proofread.config/apply-diagnostic.html">apply-diagnostic</a></li>
<li><a href="proofread.config/apply-diagnostics.html">apply-diagnostics</a></li>
<li><a href="proofread.config/config-file.html">config-file</a></li>
<li><a href="proofread.config/config-files.html">config-files</a></li>
<li><a href="proofread.config/config-path.html">config-path</a></li>
<li><a href="proofread.config/diagnostic-severity.html">diagnostic-severity</a></li>
<li><a href="proofread.config/file-config.html">file-config</a></li>
<li><a href="proofread.config/merge-config.html">merge-config</a></li>
<li><a href="proofread.config/merge-configs.html">merge-configs</a></li>
<li><a href="proofread.config/rule-enabled-p.html">rule-enabled?</a></li>
<li><a href="proofread.config/unknown-rules.html">unknown-rules</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proofread next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread.config</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proofread.config.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread.config

| Symbol                                                             | Summary                                                |
| ------------------------------------------------------------------ | ------------------------------------------------------ |
| [`apply-diagnostic`](proofread.config/apply-diagnostic.html)       | Apply proofread config to one diagnostic.              |
| [`apply-diagnostics`](proofread.config/apply-diagnostics.html)     | Apply proofread config to diagnostics.                 |
| [`config-file`](proofread.config/config-file.html)                 | Proofread configuration file name.                     |
| [`config-files`](proofread.config/config-files.html)               | Find proofread config files that apply to a directory. |
| [`config-path`](proofread.config/config-path.html)                 | Return the proofread config path for a directory.      |
| [`diagnostic-severity`](proofread.config/diagnostic-severity.html) | Return the configured severity for a diagnostic.       |
| [`file-config`](proofread.config/file-config.html)                 | Load the effective proofread config for a file.        |
| [`merge-config`](proofread.config/merge-config.html)               | Merge proofread config maps recursively.               |
| [`merge-configs`](proofread.config/merge-configs.html)             | Merge proofread config maps in order.                  |
| [`rule-enabled?`](proofread.config/rule-enabled-p.html)            | Test whether a rule is enabled by config.              |
| [`unknown-rules`](proofread.config/unknown-rules.html)             | Return configured rule IDs that are not registered.    |

</main>
</div>
