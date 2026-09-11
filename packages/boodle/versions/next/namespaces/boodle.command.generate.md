---
layout: reference
title: boodle.command.generate
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.command.generate.html">boodle.command.generate</a>
<ol>
<li><a href="boodle.command.generate/append-package-id-once.html">append-package-id-once</a></li>
<li><a href="boodle.command.generate/apply-retention-bang.html">apply-retention!</a></li>
<li><a href="boodle.command.generate/catalog-package-ids.html">catalog-package-ids</a></li>
<li><a href="boodle.command.generate/catalog-with-known-publications.html">catalog-with-known-publications</a></li>
<li><a href="boodle.command.generate/catalog-with-navigation-style.html">catalog-with-navigation-style</a></li>
<li><a href="boodle.command.generate/catalog-with-package-groups.html">catalog-with-package-groups</a></li>
<li><a href="boodle.command.generate/command.html">command</a></li>
<li><a href="boodle.command.generate/directory-entry-p.html">directory-entry?</a></li>
<li><a href="boodle.command.generate/execute-bang.html">execute!</a></li>
<li><a href="boodle.command.generate/existing-package-identities.html">existing-package-identities</a></li>
<li><a href="boodle.command.generate/expand-package-group.html">expand-package-group</a></li>
<li><a href="boodle.command.generate/explicit-package-group-ids.html">explicit-package-group-ids</a></li>
<li><a href="boodle.command.generate/generated-files.html">generated-files</a></li>
<li><a href="boodle.command.generate/package-with-known-publications.html">package-with-known-publications</a></li>
<li><a href="boodle.command.generate/parse-package-group.html">parse-package-group</a></li>
<li><a href="boodle.command.generate/read-catalog.html">read-catalog</a></li>
<li><a href="boodle.command.generate/remaining-package-ids.html">remaining-package-ids</a></li>
<li><a href="boodle.command.generate/versions-dir.html">versions-dir</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.command.generate</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.command.generate.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.command.generate

| Symbol                                                                                            | Summary                                                               |
| ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| [`append-package-id-once`](boodle.command.generate/append-package-id-once.html)                   | Append ID to IDS when it is not already present.                      |
| [`apply-retention!`](boodle.command.generate/apply-retention-bang.html)                           | Remove publication directories made obsolete by PUBLICATION.          |
| [`catalog-package-ids`](boodle.command.generate/catalog-package-ids.html)                         | Return package ids in CATALOG order.                                  |
| [`catalog-with-known-publications`](boodle.command.generate/catalog-with-known-publications.html) | Return CATALOG annotated with already materialized publications.      |
| [`catalog-with-navigation-style`](boodle.command.generate/catalog-with-navigation-style.html)     | Attach navigation style metadata to CATALOG.                          |
| [`catalog-with-package-groups`](boodle.command.generate/catalog-with-package-groups.html)         | Attach expanded package groups to CATALOG.                            |
| [`command`](boodle.command.generate/command.html)                                                 | cli-trooper declaration for Boodle's `generate` command.              |
| [`directory-entry?`](boodle.command.generate/directory-entry-p.html)                              | Return non-nil when ENTRY describes a directory.                      |
| [`execute!`](boodle.command.generate/execute-bang.html)                                           | Generate Boodle documentation from fully parsed command options.      |
| [`existing-package-identities`](boodle.command.generate/existing-package-identities.html)         | Return already materialized documentation identities for PACKAGE.     |
| [`expand-package-group`](boodle.command.generate/expand-package-group.html)                       | Expand wildcard package ids in GROUP.                                 |
| [`explicit-package-group-ids`](boodle.command.generate/explicit-package-group-ids.html)           | Return non-wildcard package ids mentioned by GROUPS.                  |
| [`generated-files`](boodle.command.generate/generated-files.html)                                 | Return documentation files for OPTIONS.                               |
| [`package-with-known-publications`](boodle.command.generate/package-with-known-publications.html) | Return PACKAGE with generated and materialized publications attached. |
| [`parse-package-group`](boodle.command.generate/parse-package-group.html)                         | Parse one Boodle package-group option value.                          |
| [`read-catalog`](boodle.command.generate/read-catalog.html)                                       | Read lookup indexes into a Boodle catalog.                            |
| [`remaining-package-ids`](boodle.command.generate/remaining-package-ids.html)                     | Return package ids not explicitly assigned to a package group.        |
| [`versions-dir`](boodle.command.generate/versions-dir.html)                                       | Return the materialized versions directory for PACKAGE in OUTPUT-DIR. |

</main>
</div>
