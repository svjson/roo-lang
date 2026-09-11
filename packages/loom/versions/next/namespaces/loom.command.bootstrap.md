---
layout: reference
title: loom.command.bootstrap
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="loom.command.bootstrap.html">loom.command.bootstrap</a>
<ol>
<li><a href="loom.command.bootstrap/bootstrap-bang.html">bootstrap!</a></li>
<li><a href="loom.command.bootstrap/install-dependencies-bang.html">install-dependencies!</a></li>
<li><a href="loom.command.bootstrap/install-node-bang.html">install-node!</a></li>
<li><a href="loom.command.bootstrap/reject-bang.html">reject!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">loom next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">loom.command.bootstrap</span>
</nav>
<div class="version-context">
<span class="version-package">loom</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["loom"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/loom.command.bootstrap.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# loom.command.bootstrap

| Symbol                                                                           | Summary                                                          |
| -------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [`bootstrap!`](loom.command.bootstrap/bootstrap-bang.html)                       | Install the source dependency closure needed to build a package. |
| [`install-dependencies!`](loom.command.bootstrap/install-dependencies-bang.html) | Install DEPENDENCIES into REPO-ROOT in dependency-first order.   |
| [`install-node!`](loom.command.bootstrap/install-node-bang.html)                 | Install NODE and its dependency subtree into REPO-ROOT.          |
| [`reject!`](loom.command.bootstrap/reject-bang.html)                             | Report a bootstrap rejection.                                    |

</main>
</div>
