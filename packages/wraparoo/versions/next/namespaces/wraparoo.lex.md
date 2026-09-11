---
layout: reference
title: wraparoo.lex
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="wraparoo.lex.html">wraparoo.lex</a>
<ol>
<li><a href="wraparoo.lex/boundary-p.html">boundary?</a></li>
<li><a href="wraparoo.lex/delimiter-specs.html">delimiter-specs</a></li>
<li><a href="wraparoo.lex/number-token-p.html">number-token?</a></li>
<li><a href="wraparoo.lex/scan.html">scan</a></li>
<li><a href="wraparoo.lex/scan-token.html">scan-token</a></li>
<li><a href="wraparoo.lex/span-end.html">span-end</a></li>
<li><a href="wraparoo.lex/string-span.html">string-span</a></li>
<li><a href="wraparoo.lex/token.html">token</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">wraparoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">wraparoo.lex</span>
</nav>
<div class="version-context">
<span class="version-package">wraparoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["wraparoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/wraparoo.lex.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# wraparoo.lex

Losslessly scan complete or incomplete Roo source into lexical tokens.

`scan` retains whitespace, comments, and exact token text so concatenating
the token text reconstructs the original source. Token ranges are half-open
Unicode-scalar offsets, and incomplete strings or escaped characters are
returned with `:status :incomplete` instead of raising an error.

### Examples:

```roo
(require '[wraparoo.lex :as lex])

(def tokens (lex/scan "(+ 1 2"))

(select-keys (head tokens) [:kind :text :range :status])
```

Most integrations can use `wraparoo/analyze`, which combines this token
stream with delimiter analysis.

| Symbol                                                 | Summary                                                                        |
| ------------------------------------------------------ | ------------------------------------------------------------------------------ |
| [`boundary?`](wraparoo.lex/boundary-p.html)            | Return whether SCALAR ends an ordinary Roo atom during source analysis.        |
| [`delimiter-specs`](wraparoo.lex/delimiter-specs.html) | Roo collection delimiter characters and their structural roles.                |
| [`number-token?`](wraparoo.lex/number-token-p.html)    | Return whether TEXT has the lexical shape of a Roo number.                     |
| [`scan`](wraparoo.lex/scan.html)                       | Scan complete or incomplete Roo SOURCE into lossless lexical tokens.           |
| [`scan-token`](wraparoo.lex/scan-token.html)           | Scan the source token beginning at START.                                      |
| [`span-end`](wraparoo.lex/span-end.html)               | Find the exclusive end of a token span while CONTINUES? accepts scalars.       |
| [`string-span`](wraparoo.lex/string-span.html)         | Analyze a string token beginning at START without requiring its closing quote. |
| [`token`](wraparoo.lex/token.html)                     | Construct one lossless source token.                                           |

</main>
</div>
