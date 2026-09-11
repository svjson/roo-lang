---
layout: reference
title: boodle.topology.reference
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.topology.reference.html">boodle.topology.reference</a>
<ol>
<li><a href="boodle.topology.reference/breadcrumbs.html">breadcrumbs</a></li>
<li><a href="boodle.topology.reference/namespace-breadcrumb.html">namespace-breadcrumb</a></li>
<li><a href="boodle.topology.reference/namespace-page-url.html">namespace-page-url</a></li>
<li><a href="boodle.topology.reference/namespace-path.html">namespace-path</a></li>
<li><a href="boodle.topology.reference/navigation-style.html">navigation-style</a></li>
<li><a href="boodle.topology.reference/package-breadcrumb.html">package-breadcrumb</a></li>
<li><a href="boodle.topology.reference/package-index-path.html">package-index-path</a></li>
<li><a href="boodle.topology.reference/package-page-url.html">package-page-url</a></li>
<li><a href="boodle.topology.reference/package-path.html">package-path</a></li>
<li><a href="boodle.topology.reference/package-title.html">package-title</a></li>
<li><a href="boodle.topology.reference/package-version.html">package-version</a></li>
<li><a href="boodle.topology.reference/package-version-root.html">package-version-root</a></li>
<li><a href="boodle.topology.reference/pages.html">pages</a></li>
<li><a href="boodle.topology.reference/root-breadcrumb.html">root-breadcrumb</a></li>
<li><a href="boodle.topology.reference/root-url.html">root-url</a></li>
<li><a href="boodle.topology.reference/slug-replacements.html">slug-replacements</a></li>
<li><a href="boodle.topology.reference/slug-text.html">slug-text</a></li>
<li><a href="boodle.topology.reference/symbol-breadcrumb.html">symbol-breadcrumb</a></li>
<li><a href="boodle.topology.reference/symbol-path.html">symbol-path</a></li>
<li><a href="boodle.topology.reference/symbolic-slugs.html">symbolic-slugs</a></li>
<li><a href="boodle.topology.reference/up-item.html">up-item</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.topology.reference</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.topology.reference.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.topology.reference

| Symbol                                                                        | Summary                                                                  |
| ----------------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| [`breadcrumbs`](boodle.topology.reference/breadcrumbs.html)                   | Return hierarchy breadcrumbs for PAGE.                                   |
| [`namespace-breadcrumb`](boodle.topology.reference/namespace-breadcrumb.html) | Return the namespace breadcrumb item for PAGE.                           |
| [`namespace-page-url`](boodle.topology.reference/namespace-page-url.html)     | Return the namespace page URL from PAGE.                                 |
| [`namespace-path`](boodle.topology.reference/namespace-path.html)             | Return the reference page path for NAMESPACE in PACKAGE.                 |
| [`navigation-style`](boodle.topology.reference/navigation-style.html)         | Return the reference navigation style for CATALOG.                       |
| [`package-breadcrumb`](boodle.topology.reference/package-breadcrumb.html)     | Return the package breadcrumb item for PAGE.                             |
| [`package-index-path`](boodle.topology.reference/package-index-path.html)     | Return the compatibility package index path for PACKAGE.                 |
| [`package-page-url`](boodle.topology.reference/package-page-url.html)         | Return the package page URL from PAGE.                                   |
| [`package-path`](boodle.topology.reference/package-path.html)                 | Return the versioned reference page path for PACKAGE.                    |
| [`package-title`](boodle.topology.reference/package-title.html)               | Return the display title for PACKAGE.                                    |
| [`package-version`](boodle.topology.reference/package-version.html)           | Return the version label for PACKAGE.                                    |
| [`package-version-root`](boodle.topology.reference/package-version-root.html) | Return the versioned reference root path for PACKAGE.                    |
| [`pages`](boodle.topology.reference/pages.html)                               | Return reference documentation pages for CATALOG.                        |
| [`root-breadcrumb`](boodle.topology.reference/root-breadcrumb.html)           | Return the root breadcrumb item for PAGE.                                |
| [`root-url`](boodle.topology.reference/root-url.html)                         | Return the root reference URL from PAGE.                                 |
| [`slug-replacements`](boodle.topology.reference/slug-replacements.html)       | Character replacements used when building symbol page paths.             |
| [`slug-text`](boodle.topology.reference/slug-text.html)                       | Return path-safe text for VALUE.                                         |
| [`symbol-breadcrumb`](boodle.topology.reference/symbol-breadcrumb.html)       | Return the symbol breadcrumb item for PAGE.                              |
| [`symbol-path`](boodle.topology.reference/symbol-path.html)                   | Return the reference page path for ENTRY in NAMESPACE.                   |
| [`symbolic-slugs`](boodle.topology.reference/symbolic-slugs.html)             | Path-safe slugs for symbol names that kebab-case would erase or collide. |
| [`up-item`](boodle.topology.reference/up-item.html)                           | Return the immediate parent navigation item for PAGE.                    |

</main>
</div>
