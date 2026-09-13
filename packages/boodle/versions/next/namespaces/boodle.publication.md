---
layout: reference
title: boodle.publication
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="boodle.publication.html">boodle.publication</a>
<ol>
<li><a href="boodle.publication/apply-to-catalog.html">apply-to-catalog</a></li>
<li><a href="boodle.publication/automatic.html">automatic</a></li>
<li><a href="boodle.publication/build-identifier-p.html">build-identifier?</a></li>
<li><a href="boodle.publication/classify.html">classify</a></li>
<li><a href="boodle.publication/compare-prerelease.html">compare-prerelease</a></li>
<li><a href="boodle.publication/compare-versions.html">compare-versions</a></li>
<li><a href="boodle.publication/identifier-character-p.html">identifier-character?</a></li>
<li><a href="boodle.publication/identifier-p.html">identifier?</a></li>
<li><a href="boodle.publication/make.html">make</a></li>
<li><a href="boodle.publication/numeric-identifier-p.html">numeric-identifier?</a></li>
<li><a href="boodle.publication/obsolete-identities.html">obsolete-identities</a></li>
<li><a href="boodle.publication/ordered.html">ordered</a></li>
<li><a href="boodle.publication/parse-version.html">parse-version</a></li>
<li><a href="boodle.publication/retention-errors.html">retention-errors</a></li>
<li><a href="boodle.publication/validation-errors.html">validation-errors</a></li>
<li><a href="boodle.publication/version-core.html">version-core</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">boodle next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">boodle.publication</span>
</nav>
<div class="version-context">
<span class="version-package">boodle</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["boodle"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/boodle.publication.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# boodle.publication

| Symbol                                                                    | Summary                                                            |
| ------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| [`apply-to-catalog`](boodle.publication/apply-to-catalog.html)            | Apply a public documentation identity to every package in CATALOG. |
| [`automatic`](boodle.publication/automatic.html)                          | Select the automatic publication.                                  |
| [`build-identifier?`](boodle.publication/build-identifier-p.html)         | Query whether TEXT is a valid SemVer build identifier.             |
| [`classify`](boodle.publication/classify.html)                            | Classify a materialized public documentation identity.             |
| [`compare-prerelease`](boodle.publication/compare-prerelease.html)        | Compare two SemVer prerelease identifier vectors.                  |
| [`compare-versions`](boodle.publication/compare-versions.html)            | Compare two parsed semantic versions.                              |
| [`identifier-character?`](boodle.publication/identifier-character-p.html) | Query whether CHARACTER is allowed in a SemVer identifier.         |
| [`identifier?`](boodle.publication/identifier-p.html)                     | Query whether TEXT is a valid SemVer prerelease identifier.        |
| [`make`](boodle.publication/make.html)                                    | Create a validated documentation publication identity.             |
| [`numeric-identifier?`](boodle.publication/numeric-identifier-p.html)     | Query whether TEXT is a valid SemVer numeric identifier.           |
| [`obsolete-identities`](boodle.publication/obsolete-identities.html)      | Return materialized identities removed by PUBLICATION.             |
| [`ordered`](boodle.publication/ordered.html)                              | Return classified publication identities in selector order.        |
| [`parse-version`](boodle.publication/parse-version.html)                  | Parse a semantic version.                                          |
| [`retention-errors`](boodle.publication/retention-errors.html)            | Validate a publication against already materialized identities.    |
| [`validation-errors`](boodle.publication/validation-errors.html)          | Validate parsed publication command options.                       |
| [`version-core`](boodle.publication/version-core.html)                    | Return the major, minor, and patch identity of VERSION.            |

</main>
</div>
