---
layout: reference
title: boodle.format.github-pages/package-index-url
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../boodle.format.github-pages.html">boodle.format.github-pages</a>
<ol>
<li><a href="breadcrumb-item-html.html">breadcrumb-item-html</a></li>
<li><a href="breadcrumb-items-html.html">breadcrumb-items-html</a></li>
<li><a href="breadcrumbs-html.html">breadcrumbs-html</a></li>
<li><a href="content-blocks.html">content-blocks</a></li>
<li><a href="documentation-text.html">documentation-text</a></li>
<li><a href="entry-title.html">entry-title</a></li>
<li><a href="entry-title-code.html">entry-title-code</a></li>
<li><a href="files.html">files</a></li>
<li><a href="front-matter.html">front-matter</a></li>
<li><a href="html-text.html">html-text</a></li>
<li><a href="index-document.html">index-document</a></li>
<li><a href="layout-content.html">layout-content</a></li>
<li><a href="namespace-document.html">namespace-document</a></li>
<li><a href="namespace-overview-url.html">namespace-overview-url</a></li>
<li><a href="namespace-symbol-url.html">namespace-symbol-url</a></li>
<li><a href="navigation-html.html">navigation-html</a></li>
<li><a href="navigation-link-html.html">navigation-link-html</a></li>
<li><a href="package-document.html">package-document</a></li>
<li><a href="package-group-blocks.html">package-group-blocks</a></li>
<li><a href="package-index-blocks.html">package-index-blocks</a></li>
<li><a href="package-index-document.html">package-index-document</a></li>
<li><a href="package-index-redirect-html.html">package-index-redirect-html</a></li>
<li><a class="active" href="package-index-url.html">package-index-url</a></li>
<li><a href="package-publications.html">package-publications</a></li>
<li><a href="package-row.html">package-row</a></li>
<li><a href="package-section-blocks.html">package-section-blocks</a></li>
<li><a href="package-table.html">package-table</a></li>
<li><a href="package-title.html">package-title</a></li>
<li><a href="package-url.html">package-url</a></li>
<li><a href="package-version.html">package-version</a></li>
<li><a href="package-with-id.html">package-with-id</a></li>
<li><a href="packages-with-ids.html">packages-with-ids</a></li>
<li><a href="page-document.html">page-document</a></li>
<li><a href="page-title.html">page-title</a></li>
<li><a href="publication-data-content.html">publication-data-content</a></li>
<li><a href="reference-content.html">reference-content</a></li>
<li><a href="render-page.html">render-page</a></li>
<li><a href="sidebar-html.html">sidebar-html</a></li>
<li><a href="sidebar-symbol-link.html">sidebar-symbol-link</a></li>
<li><a href="sidebar-symbol-url.html">sidebar-symbol-url</a></li>
<li><a href="signature-args-blocks.html">signature-args-blocks</a></li>
<li><a href="signature-params.html">signature-params</a></li>
<li><a href="signature-return-blocks.html">signature-return-blocks</a></li>
<li><a href="stylesheet-content.html">stylesheet-content</a></li>
<li><a href="symbol-document.html">symbol-document</a></li>
<li><a href="symbol-heading-html.html">symbol-heading-html</a></li>
<li><a href="unplaced-signature-blocks.html">unplaced-signature-blocks</a></li>
<li><a href="up-navigation-html.html">up-navigation-html</a></li>
<li><a href="version-context-html.html">version-context-html</a></li>
<li><a href="version-page-url.html">version-page-url</a></li>
<li><a href="version-selector-html.html">version-selector-html</a></li>
<li><a href="yaml-text.html">yaml-text</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../boodle.format.github-pages.html">boodle.format.github-pages</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">package-index-url</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/boodle.format.github-pages/package-index-url.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../boodle.format.github-pages.html">boodle.format.github-pages</a>/</div>
<h1><code>package-index-url</code></h1>
</header>

Return the current-version URL from a package index redirect page.

<hr class="signature-divider">

```roo
(package-index-url package)
```

<hr class="signature-divider">

## Signature:

```roo
(package-index-url package)
```

| Arg     | Description         |
| ------- | ------------------- |
| package | Boodle package map. |

### Returns:

Relative URL to the automatically selected package version page.

</main>
</div>
