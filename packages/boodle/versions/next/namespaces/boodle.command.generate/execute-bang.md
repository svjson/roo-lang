---
layout: reference
title: boodle.command.generate/execute!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.command.generate.html">boodle.command.generate</a>
<ol>
<li><a href="append-package-id-once.html">append-package-id-once</a></li>
<li><a href="apply-retention-bang.html">apply-retention!</a></li>
<li><a href="catalog-package-ids.html">catalog-package-ids</a></li>
<li><a href="catalog-with-known-publications.html">catalog-with-known-publications</a></li>
<li><a href="catalog-with-navigation-style.html">catalog-with-navigation-style</a></li>
<li><a href="catalog-with-package-groups.html">catalog-with-package-groups</a></li>
<li><a href="command.html">command</a></li>
<li><a href="directory-entry-p.html">directory-entry?</a></li>
<li><a class="active" href="execute-bang.html">execute!</a></li>
<li><a href="existing-package-identities.html">existing-package-identities</a></li>
<li><a href="expand-package-group.html">expand-package-group</a></li>
<li><a href="explicit-package-group-ids.html">explicit-package-group-ids</a></li>
<li><a href="generated-files.html">generated-files</a></li>
<li><a href="package-with-known-publications.html">package-with-known-publications</a></li>
<li><a href="parse-package-group.html">parse-package-group</a></li>
<li><a href="read-catalog.html">read-catalog</a></li>
<li><a href="remaining-package-ids.html">remaining-package-ids</a></li>
<li><a href="versions-dir.html">versions-dir</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.command.generate.html">boodle.command.generate</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">execute!</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.command.generate/execute-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.command.generate.html">boodle.command.generate</a>/</div>
<h1><code>execute!</code></h1>
</header>

Generate Boodle documentation from fully parsed command options.

<hr class="signature-divider">

```roo
(execute! context options)
```

<hr class="signature-divider">

## Signature:

```roo
(execute! context options)
```

| Arg     | Description                                                                                                            |
| ------- | ---------------------------------------------------------------------------------------------------------------------- |
| context | The cli-trooper invocation context. An optional<br>`:cli-trooper/print!` function replaces the default output printer. |
| options | Parsed Boodle generate options.                                                                                        |

### Returns:

A result map containing the output line and written file maps.

</main>
</div>
