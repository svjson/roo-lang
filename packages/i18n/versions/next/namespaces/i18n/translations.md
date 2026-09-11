---
layout: reference
title: i18n/translations
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../i18n.html">i18n</a>
<ol>
<li><a href="catalog.html">catalog</a></li>
<li><a href="default-locale.html">default-locale</a></li>
<li><a href="fallback-message.html">fallback-message</a></li>
<li><a href="locale-messages.html">locale-messages</a></li>
<li><a href="message.html">message</a></li>
<li><a href="missing-p.html">missing?</a></li>
<li><a href="translate.html">translate</a></li>
<li><a href="translated-p.html">translated?</a></li>
<li><a class="active" href="translations.html">translations</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">i18n next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../i18n.html">i18n</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">translations</span>
</nav>
<div class="version-context">
<span class="version-package">i18n</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["i18n"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/i18n/translations.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../i18n.html">i18n</a>/</div>
<h1><code>translations</code></h1>
</header>

Return a catalog's translation table.

<hr class="signature-divider">

```roo
(translations catalog)
```

<hr class="signature-divider">

## Signature:

```roo
(translations catalog)
```

| Arg     | Description              |
| ------- | ------------------------ |
| catalog | Translation catalog map. |

### Returns:

A map from locale identifiers to message maps, or an empty map when absent.

</main>
</div>
