---
layout: reference
title: proofread next
---

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">proofread next</span>
</nav>
<div class="version-context">
<span class="version-package">proofread</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["proofread"].publications %}
{% for publication in package_publications %}
<option value="../{{ publication.identity }}/"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# proofread next

Roo syntax and style checker.

| Namespace                                                                                                                                      | Symbols |
| ---------------------------------------------------------------------------------------------------------------------------------------------- | ------- |
| [proofread.cli](namespaces/proofread.cli.html)                                                                                                 | 3       |
| [proofread.command.check](namespaces/proofread.command.check.html)                                                                             | 8       |
| [proofread.command.suggest](namespaces/proofread.command.suggest.html)                                                                         | 9       |
| [proofread.config](namespaces/proofread.config.html)                                                                                           | 11      |
| [proofread.inspect.bindings](namespaces/proofread.inspect.bindings.html)                                                                       | 8       |
| [proofread.inspect.expressions](namespaces/proofread.inspect.expressions.html)                                                                 | 8       |
| [proofread.inspect.forms](namespaces/proofread.inspect.forms.html)                                                                             | 33      |
| [proofread.inspect.namespaces](namespaces/proofread.inspect.namespaces.html)                                                                   | 2       |
| [proofread.inspect.packages](namespaces/proofread.inspect.packages.html)                                                                       | 1       |
| [proofread.inspect.sequences](namespaces/proofread.inspect.sequences.html)                                                                     | 10      |
| [proofread.inspection](namespaces/proofread.inspection.html)                                                                                   | 3       |
| [proofread.reporter](namespaces/proofread.reporter.html)                                                                                       | 3       |
| [proofread.reporter.common](namespaces/proofread.reporter.common.html)                                                                         | 19      |
| [proofread.reporter.grouped](namespaces/proofread.reporter.grouped.html)                                                                       | 12      |
| [proofread.reporter.simple](namespaces/proofread.reporter.simple.html)                                                                         | 3       |
| [proofread.rules](namespaces/proofread.rules.html)                                                                                             | 6       |
| [proofread.rules.diagnostics](namespaces/proofread.rules.diagnostics.html)                                                                     | 6       |
| [proofread.rules.direct-conditions](namespaces/proofread.rules.direct-conditions.html)                                                         | 5       |
| [proofread.rules.direct-conditions.flatten-if-pyramid](namespaces/proofread.rules.direct-conditions.flatten-if-pyramid.html)                   | 2       |
| [proofread.rules.direct-conditions.redundant-self-binding](namespaces/proofread.rules.direct-conditions.redundant-self-binding.html)           | 3       |
| [proofread.rules.direct-conditions.use-case](namespaces/proofread.rules.direct-conditions.use-case.html)                                       | 7       |
| [proofread.rules.direct-conditions.use-cond](namespaces/proofread.rules.direct-conditions.use-cond.html)                                       | 3       |
| [proofread.rules.direct-conditions.use-cond-thread-first](namespaces/proofread.rules.direct-conditions.use-cond-thread-first.html)             | 6       |
| [proofread.rules.direct-conditions.use-if-let](namespaces/proofread.rules.direct-conditions.use-if-let.html)                                   | 2       |
| [proofread.rules.direct-conditions.use-unless](namespaces/proofread.rules.direct-conditions.use-unless.html)                                   | 4       |
| [proofread.rules.direct-conditions.use-when](namespaces/proofread.rules.direct-conditions.use-when.html)                                       | 1       |
| [proofread.rules.direct-conditions.use-when-let](namespaces/proofread.rules.direct-conditions.use-when-let.html)                               | 5       |
| [proofread.rules.higher-order](namespaces/proofread.rules.higher-order.html)                                                                   | 2       |
| [proofread.rules.higher-order.use-find-first](namespaces/proofread.rules.higher-order.use-find-first.html)                                     | 2       |
| [proofread.rules.higher-order.use-flat-map](namespaces/proofread.rules.higher-order.use-flat-map.html)                                         | 2       |
| [proofread.rules.higher-order.use-keep](namespaces/proofread.rules.higher-order.use-keep.html)                                                 | 4       |
| [proofread.rules.loops](namespaces/proofread.rules.loops.html)                                                                                 | 2       |
| [proofread.rules.loops.use-for-indexed](namespaces/proofread.rules.loops.use-for-indexed.html)                                                 | 11      |
| [proofread.rules.loops.use-reduce](namespaces/proofread.rules.loops.use-reduce.html)                                                           | 8       |
| [proofread.rules.names](namespaces/proofread.rules.names.html)                                                                                 | 3       |
| [proofread.rules.names.kebab-case-filename](namespaces/proofread.rules.names.kebab-case-filename.html)                                         | 2       |
| [proofread.rules.names.kebab-case-name](namespaces/proofread.rules.names.kebab-case-name.html)                                                 | 2       |
| [proofread.rules.redundant-do](namespaces/proofread.rules.redundant-do.html)                                                                   | 2       |
| [proofread.rules.redundant-do.redundant-do](namespaces/proofread.rules.redundant-do.redundant-do.html)                                         | 4       |
| [proofread.rules.redundant-functions](namespaces/proofread.rules.redundant-functions.html)                                                     | 2       |
| [proofread.rules.redundant-functions.redundant-function](namespaces/proofread.rules.redundant-functions.redundant-function.html)               | 16      |
| [proofread.rules.registry](namespaces/proofread.rules.registry.html)                                                                           | 7       |
| [proofread.rules.simple-expressions](namespaces/proofread.rules.simple-expressions.html)                                                       | 2       |
| [proofread.rules.simple-expressions.flatten-struct-operations](namespaces/proofread.rules.simple-expressions.flatten-struct-operations.html)   | 2       |
| [proofread.rules.simple-expressions.redundant-empty-default](namespaces/proofread.rules.simple-expressions.redundant-empty-default.html)       | 3       |
| [proofread.rules.simple-expressions.redundant-let](namespaces/proofread.rules.simple-expressions.redundant-let.html)                           | 2       |
| [proofread.rules.simple-expressions.redundant-let-binding](namespaces/proofread.rules.simple-expressions.redundant-let-binding.html)           | 2       |
| [proofread.rules.simple-expressions.redundant-let-rebind](namespaces/proofread.rules.simple-expressions.redundant-let-rebind.html)             | 9       |
| [proofread.rules.simple-expressions.redundant-map-check](namespaces/proofread.rules.simple-expressions.redundant-map-check.html)               | 6       |
| [proofread.rules.simple-expressions.redundant-some](namespaces/proofread.rules.simple-expressions.redundant-some.html)                         | 2       |
| [proofread.rules.simple-expressions.use-any](namespaces/proofread.rules.simple-expressions.use-any.html)                                       | 5       |
| [proofread.rules.simple-expressions.use-append](namespaces/proofread.rules.simple-expressions.use-append.html)                                 | 4       |
| [proofread.rules.simple-expressions.use-argument-destructuring](namespaces/proofread.rules.simple-expressions.use-argument-destructuring.html) | 6       |
| [proofread.rules.simple-expressions.use-clamp](namespaces/proofread.rules.simple-expressions.use-clamp.html)                                   | 2       |
| [proofread.rules.simple-expressions.use-contains](namespaces/proofread.rules.simple-expressions.use-contains.html)                             | 7       |
| [proofread.rules.simple-expressions.use-empty](namespaces/proofread.rules.simple-expressions.use-empty.html)                                   | 4       |
| [proofread.rules.simple-expressions.use-for](namespaces/proofread.rules.simple-expressions.use-for.html)                                       | 2       |
| [proofread.rules.simple-expressions.use-get-in-with-default](namespaces/proofread.rules.simple-expressions.use-get-in-with-default.html)       | 2       |
| [proofread.rules.simple-expressions.use-get-with-default](namespaces/proofread.rules.simple-expressions.use-get-with-default.html)             | 3       |
| [proofread.rules.simple-expressions.use-head](namespaces/proofread.rules.simple-expressions.use-head.html)                                     | 2       |
| [proofread.rules.simple-expressions.use-keyword-lookup](namespaces/proofread.rules.simple-expressions.use-keyword-lookup.html)                 | 6       |
| [proofread.rules.simple-expressions.use-nil](namespaces/proofread.rules.simple-expressions.use-nil.html)                                       | 4       |
| [proofread.rules.simple-expressions.use-not-empty](namespaces/proofread.rules.simple-expressions.use-not-empty.html)                           | 8       |
| [proofread.rules.simple-expressions.use-not-equal](namespaces/proofread.rules.simple-expressions.use-not-equal.html)                           | 4       |
| [proofread.rules.simple-expressions.use-select-keys](namespaces/proofread.rules.simple-expressions.use-select-keys.html)                       | 4       |
| [proofread.rules.simple-expressions.use-slice](namespaces/proofread.rules.simple-expressions.use-slice.html)                                   | 2       |
| [proofread.rules.simple-expressions.use-some](namespaces/proofread.rules.simple-expressions.use-some.html)                                     | 5       |
| [proofread.rules.simple-expressions.use-string](namespaces/proofread.rules.simple-expressions.use-string.html)                                 | 4       |
| [proofread.rules.simple-expressions.use-string-starts-with](namespaces/proofread.rules.simple-expressions.use-string-starts-with.html)         | 8       |
| [proofread.rules.simple-expressions.use-type-predicate](namespaces/proofread.rules.simple-expressions.use-type-predicate.html)                 | 6       |
| [proofread.rules.simple-expressions.use-update](namespaces/proofread.rules.simple-expressions.use-update.html)                                 | 7       |
| [proofread.source](namespaces/proofread.source.html)                                                                                           | 5       |
| [proofread.source.walk](namespaces/proofread.source.walk.html)                                                                                 | 23      |
