---
layout: reference
title: wraparoo.syntax
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="wraparoo.syntax.html">wraparoo.syntax</a>
<ol>
<li><a href="wraparoo.syntax/begin-element.html">begin-element</a></li>
<li><a href="wraparoo.syntax/classify.html">classify</a></li>
<li><a href="wraparoo.syntax/constants.html">constants</a></li>
<li><a href="wraparoo.syntax/lexical-ranges.html">lexical-ranges</a></li>
<li><a href="wraparoo.syntax/local-name-range.html">local-name-range</a></li>
<li><a href="wraparoo.syntax/special-forms.html">special-forms</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wraparoo.syntax</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/wraparoo.syntax.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# wraparoo.syntax

Classify lexical tokens by their source-level Roo syntax roles.

`classify` works from Wraparoo's lossless token stream and returns neutral,
typed source ranges. It recognizes lexical roles and the structural roles
available from incomplete source, including call heads, definition names,
and docstrings. It does not resolve symbols or choose presentation styles.

| Symbol                                                      | Summary                                                                    |
| ----------------------------------------------------------- | -------------------------------------------------------------------------- |
| [`begin-element`](wraparoo.syntax/begin-element.html)       | Record TOKEN as the next direct element of the current source container.   |
| [`classify`](wraparoo.syntax/classify.html)                 | Classify complete or incomplete Roo TOKENS into typed syntax ranges.       |
| [`constants`](wraparoo.syntax/constants.html)               | Vector of unqualified Roo symbol strings that represent literal constants. |
| [`lexical-ranges`](wraparoo.syntax/lexical-ranges.html)     | Classify TOKEN using lexical facts independent of its containing form.     |
| [`local-name-range`](wraparoo.syntax/local-name-range.html) | Return the local-name portion of symbol TOKEN.                             |
| [`special-forms`](wraparoo.syntax/special-forms.html)       | Vector of unqualified Roo call-head strings with special-form semantics.   |

</main>
</div>
