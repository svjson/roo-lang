---
layout: reference
title: proof.scenario/then
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link" href="../proof.scenario.html">proof.scenario</a>
<ol>
<li><a href="given.html">given</a></li>
<li><a class="active" href="then.html">then</a></li>
<li><a href="when.html">when</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../../">proof next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../proof.scenario.html">proof.scenario</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">then</span>
</nav>
<div class="version-context">
<span class="version-package">proof</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proof"].publications %}
{% for publication in package_publications %}
<option value="../../../{{ publication.identity }}/namespaces/proof.scenario/then.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
<header class="symbol-heading">
<div class="symbol-namespace"><a href="../proof.scenario.html">proof.scenario</a>/</div>
<h1><code>then</code></h1>
<div class="symbol-metadata"><span>Since <code>0.1.0</code></span> <span class="symbol-metadata-separator" aria-hidden="true">·</span> <span>See also: <code>given</code>, <code>when</code></span></div>
</header>

Observes the current state of a test scenario.



`then` may only appear as a top-level `deftest` phase. Its parameter vector
receives the current state and may also bind the original `given` value. It
does not replace the state used by subsequent phases.

- `params`: Argument vector binding current and optional original state.
- `body...`: Observation and assertion forms.

### Examples:

```roo
(then [{:keys [revision]} original]
(is (= revision 2))
(is (= original {:revision 1})))
```



</main>
</div>
