---
layout: reference
title: proof.fixture
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="proof.fixture.html">proof.fixture</a>
<ol>
<li><a href="proof.fixture/cache-fixture-value.html">cache-fixture-value</a></li>
<li><a href="proof.fixture/clear-bang.html">clear!</a></li>
<li><a href="proof.fixture/configure-bang.html">configure!</a></li>
<li><a href="proof.fixture/deffixture.html">deffixture</a></li>
<li><a href="proof.fixture/fixture-path.html">fixture-path</a></li>
<li><a href="proof.fixture/fixture-value.html">fixture-value</a></li>
<li><a href="proof.fixture/fixtures.html">fixtures</a></li>
<li><a href="proof.fixture/persistent-fixture-value.html">persistent-fixture-value</a></li>
<li><a href="proof.fixture/register-fixture-bang.html">register-fixture!</a></li>
<li><a href="proof.fixture/using-cache-fixture.html">using-cache-fixture</a></li>
<li><a href="proof.fixture/using-fixtures.html">using-fixtures</a></li>
<li><a href="proof.fixture/using-persistent-fixture.html">using-persistent-fixture</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proof.fixture</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/proof.fixture.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proof.fixture

| Symbol                                                                    | Summary                                                                 |
| ------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| [`cache-fixture-value`](proof.fixture/cache-fixture-value.html)           | Returns the value used by `using-cache-fixture`.                        |
| [`clear!`](proof.fixture/clear-bang.html)                                 | Clears in-process fixture cache state.                                  |
| [`configure!`](proof.fixture/configure-bang.html)                         | Configures persistent fixture storage.                                  |
| [`deffixture`](proof.fixture/deffixture.html)                             | Declares a named fixture provider.                                      |
| [`fixture-path`](proof.fixture/fixture-path.html)                         | Returns the active persistent fixture path for a relative fixture path. |
| [`fixture-value`](proof.fixture/fixture-value.html)                       | Returns the value for a declared fixture.                               |
| [`fixtures`](proof.fixture/fixtures.html)                                 | Declared fixtures keyed by fixture symbol.                              |
| [`persistent-fixture-value`](proof.fixture/persistent-fixture-value.html) | Returns the value used by `using-persistent-fixture`.                   |
| [`register-fixture!`](proof.fixture/register-fixture-bang.html)           | Registers a named fixture provider.                                     |
| [`using-cache-fixture`](proof.fixture/using-cache-fixture.html)           | Binds an in-process cache fixture value for the scope of the body.      |
| [`using-fixtures`](proof.fixture/using-fixtures.html)                     | Binds declared fixture values for the scope of the body.                |
| [`using-persistent-fixture`](proof.fixture/using-persistent-fixture.html) | Binds a persistent fixture value for the scope of the body.             |

</main>
</div>
