---
layout: reference
title: proof.fixture/using-fixtures
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.fixture.html">proof.fixture</a>
<ol>
<li><a href="cache-fixture-value.html">cache-fixture-value</a></li>
<li><a href="clear-bang.html">clear!</a></li>
<li><a href="configure-bang.html">configure!</a></li>
<li><a href="deffixture.html">deffixture</a></li>
<li><a href="fixture-path.html">fixture-path</a></li>
<li><a href="fixture-value.html">fixture-value</a></li>
<li><a href="fixtures.html">fixtures</a></li>
<li><a href="persistent-fixture-value.html">persistent-fixture-value</a></li>
<li><a href="register-fixture-bang.html">register-fixture!</a></li>
<li><a href="using-cache-fixture.html">using-cache-fixture</a></li>
<li><a class="active" href="using-fixtures.html">using-fixtures</a></li>
<li><a href="using-persistent-fixture.html">using-persistent-fixture</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.fixture.html">proof.fixture</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">using-fixtures</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.fixture/using-fixtures.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.fixture.html">proof.fixture</a>/</div>
<h1><code>using-fixtures</code></h1>
</header>

Binds declared fixture values for the scope of the body.



### Usage:

```roo
(using-fixtures [[bootstrap commonborn-bootstrap]
simple-fixture]
body...)
```

Each fixture spec may be a fixture symbol, `[binding fixture]`, or
`[binding fixture opts]`.



</main>
</div>
