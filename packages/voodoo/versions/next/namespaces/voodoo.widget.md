---
layout: reference
title: voodoo.widget
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="voodoo.widget.html">voodoo.widget</a>
<ol>
<li><a href="voodoo.widget/count-char.html">count-char</a></li>
<li><a href="voodoo.widget/line-count.html">line-count</a></li>
<li><a href="voodoo.widget/make.html">make</a></li>
<li><a href="voodoo.widget/replaced.html">replaced</a></li>
<li><a href="voodoo.widget/silent.html">silent</a></li>
<li><a href="voodoo.widget/tty-only.html">tty-only</a></li>
<li><a href="voodoo.widget/visible-once.html">visible-once</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">voodoo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">voodoo.widget</span>
</nav>
<div class="version-context">
<span class="version-package">voodoo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["voodoo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/voodoo.widget.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# voodoo.widget

| Symbol                                            | Summary                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| ------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`count-char`](voodoo.widget/count-char.html)     | Count occurrences of the single character CH in TEXT.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| [`line-count`](voodoo.widget/line-count.html)     | Number of terminal rows TEXT occupies: its embedded newline count, plus<br>one. `:text` isn't always a single line - a failing test's result line<br>folded together with its failure-detail lines, as one widget, is a<br>multi-line example - so callers that compute cursor movement (see<br>`voodoo.session`) measure a widget's height with this instead of<br>assuming one row per widget.                                                                                                                                                                                                          |
| [`make`](voodoo.widget/make.html)                 | Build a widget spec with a complete append-mode degrade policy.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| [`replaced`](voodoo.widget/replaced.html)         | Append-mode degrade policy for a widget whose `add!`-time text is a<br>placeholder not worth logging (e.g. "RUNNING x"), but whose first<br>`update!` carries its real, final content (e.g. "PASS x") - the one<br>line append mode should see. For a widget that starts in-progress and<br>is updated exactly once, to its finished state.                                                                                                                                                                                                                                                               |
| [`silent`](voodoo.widget/silent.html)             | Append-mode degrade policy for a widget that never prints anything on<br>its own in append mode, at any lifecycle point. For a widget with no<br>textual form worth logging at all - e.g. a spinner frame, meaningless<br>outside a live tty.                                                                                                                                                                                                                                                                                                                                                             |
| [`tty-only`](voodoo.widget/tty-only.html)         | Append-mode degrade policy for a widget that doesn't exist outside a<br>live tty at all: not shown, and not even registered in the session, so<br>`update!`/`remove!` calls against its handle are no-ops in append mode<br>- there's nothing there to find, not a policy decision either of them<br>has to consult. For an in-progress indicator (e.g. "RUNNING x") whose<br>caller prints its own separate, final line once the widget is removed<br>- the in-progress line itself has no place in a static log, and the<br>caller never has to ask whether it's running in a tty to make that<br>true. |
| [`visible-once`](voodoo.widget/visible-once.html) | Default append-mode degrade policy: shown once when added, silent on<br>every update or removal after that. What a one-off transient status<br>line (e.g. "Discovering proof tests...") needs - worth a single line<br>in a static log, not a scrolling transcript of every change made to it<br>while a tty was watching.                                                                                                                                                                                                                                                                                |

</main>
</div>
