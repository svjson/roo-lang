---
layout: reference
title: roo.tty/dimensions!
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../roo.tty.html">roo.tty</a>
<ol>
<li><a class="active" href="dimensions-bang.html">dimensions!</a></li>
<li><a href="tty-p.html">tty?</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../roo.tty.html">roo.tty</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">dimensions!</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/roo.tty/dimensions-bang.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../roo.tty.html">roo.tty</a>/</div>
<h1><code>dimensions!</code></h1>
</header>

Query the terminal's current dimensions.

<hr class="signature-divider">

```roo
(dimensions!)
```

<hr class="signature-divider">

The returned map contains :columns and :rows. Returns nil when stdout
is not a real terminal, or the dimensions cannot be determined.

### Usage:

```roo
(roo.tty/dimensions!) => {:columns 80 :rows 24}
(roo.tty/dimensions!) => nil
```

<hr class="signature-divider">

## Signature:

```roo
(dimensions!)
```

</main>
</div>
