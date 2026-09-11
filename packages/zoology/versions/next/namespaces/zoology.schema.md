---
layout: reference
title: zoology.schema
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="zoology.schema.html">zoology.schema</a>
<ol>
<li><a href="zoology.schema/active-schema-field-ids.html">active-schema-field-ids</a></li>
<li><a href="zoology.schema/active-schema-fields.html">active-schema-fields</a></li>
<li><a href="zoology.schema/discriminator-field-id.html">discriminator-field-id</a></li>
<li><a href="zoology.schema/dynamic-field-visibility-p.html">dynamic-field-visibility?</a></li>
<li><a href="zoology.schema/extension-base-schema.html">extension-base-schema</a></li>
<li><a href="zoology.schema/field-path.html">field-path</a></li>
<li><a href="zoology.schema/field-value.html">field-value</a></li>
<li><a href="zoology.schema/field-visibility-value.html">field-visibility-value</a></li>
<li><a href="zoology.schema/field-visible-p.html">field-visible?</a></li>
<li><a href="zoology.schema/merge-schema-nodes.html">merge-schema-nodes</a></li>
<li><a href="zoology.schema/narrow-union-schema.html">narrow-union-schema</a></li>
<li><a href="zoology.schema/path-present-at-p.html">path-present-at?</a></li>
<li><a href="zoology.schema/path-present-p.html">path-present?</a></li>
<li><a href="zoology.schema/path-value.html">path-value</a></li>
<li><a href="zoology.schema/prepare-root-schema.html">prepare-root-schema</a></li>
<li><a href="zoology.schema/prepare-schema.html">prepare-schema</a></li>
<li><a href="zoology.schema/resolve-fields.html">resolve-fields</a></li>
<li><a href="zoology.schema/resolve-nested-schema.html">resolve-nested-schema</a></li>
<li><a href="zoology.schema/resolve-schema.html">resolve-schema</a></li>
<li><a href="zoology.schema/resolve-schema-node.html">resolve-schema-node</a></li>
<li><a href="zoology.schema/resolve-variants.html">resolve-variants</a></li>
<li><a href="zoology.schema/resolved-field-schema.html">resolved-field-schema</a></li>
<li><a href="zoology.schema/resolved-root-schema.html">resolved-root-schema</a></li>
<li><a href="zoology.schema/root-field-map-p.html">root-field-map?</a></li>
<li><a href="zoology.schema/schema-definitions.html">schema-definitions</a></li>
<li><a href="zoology.schema/schema-document-with-root.html">schema-document-with-root</a></li>
<li><a href="zoology.schema/schema-field-ids.html">schema-field-ids</a></li>
<li><a href="zoology.schema/schema-fields.html">schema-fields</a></li>
<li><a href="zoology.schema/schema-node-with-discriminator-values.html">schema-node-with-discriminator-values</a></li>
<li><a href="zoology.schema/schema-root.html">schema-root</a></li>
<li><a href="zoology.schema/selected-union-variant-ids.html">selected-union-variant-ids</a></li>
<li><a href="zoology.schema/union-discriminator-value.html">union-discriminator-value</a></li>
<li><a href="zoology.schema/union-fields.html">union-fields</a></li>
<li><a href="zoology.schema/union-variant-schema.html">union-variant-schema</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">zoology next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">zoology.schema</span>
</nav>
<div class="version-context">
<span class="version-package">zoology</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["zoology"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/zoology.schema.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# zoology.schema

| Symbol                                                                                               | Summary |
| ---------------------------------------------------------------------------------------------------- | ------- |
| [`active-schema-field-ids`](zoology.schema/active-schema-field-ids.html)                             |         |
| [`active-schema-fields`](zoology.schema/active-schema-fields.html)                                   |         |
| [`discriminator-field-id`](zoology.schema/discriminator-field-id.html)                               |         |
| [`dynamic-field-visibility?`](zoology.schema/dynamic-field-visibility-p.html)                        |         |
| [`extension-base-schema`](zoology.schema/extension-base-schema.html)                                 |         |
| [`field-path`](zoology.schema/field-path.html)                                                       |         |
| [`field-value`](zoology.schema/field-value.html)                                                     |         |
| [`field-visibility-value`](zoology.schema/field-visibility-value.html)                               |         |
| [`field-visible?`](zoology.schema/field-visible-p.html)                                              |         |
| [`merge-schema-nodes`](zoology.schema/merge-schema-nodes.html)                                       |         |
| [`narrow-union-schema`](zoology.schema/narrow-union-schema.html)                                     |         |
| [`path-present-at?`](zoology.schema/path-present-at-p.html)                                          |         |
| [`path-present?`](zoology.schema/path-present-p.html)                                                |         |
| [`path-value`](zoology.schema/path-value.html)                                                       |         |
| [`prepare-root-schema`](zoology.schema/prepare-root-schema.html)                                     |         |
| [`prepare-schema`](zoology.schema/prepare-schema.html)                                               |         |
| [`resolve-fields`](zoology.schema/resolve-fields.html)                                               |         |
| [`resolve-nested-schema`](zoology.schema/resolve-nested-schema.html)                                 |         |
| [`resolve-schema`](zoology.schema/resolve-schema.html)                                               |         |
| [`resolve-schema-node`](zoology.schema/resolve-schema-node.html)                                     |         |
| [`resolve-variants`](zoology.schema/resolve-variants.html)                                           |         |
| [`resolved-field-schema`](zoology.schema/resolved-field-schema.html)                                 |         |
| [`resolved-root-schema`](zoology.schema/resolved-root-schema.html)                                   |         |
| [`root-field-map?`](zoology.schema/root-field-map-p.html)                                            |         |
| [`schema-definitions`](zoology.schema/schema-definitions.html)                                       |         |
| [`schema-document-with-root`](zoology.schema/schema-document-with-root.html)                         |         |
| [`schema-field-ids`](zoology.schema/schema-field-ids.html)                                           |         |
| [`schema-fields`](zoology.schema/schema-fields.html)                                                 |         |
| [`schema-node-with-discriminator-values`](zoology.schema/schema-node-with-discriminator-values.html) |         |
| [`schema-root`](zoology.schema/schema-root.html)                                                     |         |
| [`selected-union-variant-ids`](zoology.schema/selected-union-variant-ids.html)                       |         |
| [`union-discriminator-value`](zoology.schema/union-discriminator-value.html)                         |         |
| [`union-fields`](zoology.schema/union-fields.html)                                                   |         |
| [`union-variant-schema`](zoology.schema/union-variant-schema.html)                                   |         |

</main>
</div>
