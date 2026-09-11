---
layout: reference
title: roo.io
---

<div class="reference-page">
<nav class="reference-sidebar">
<a class="namespace-link active" href="roo.io.html">roo.io</a>
<ol>
<li><a href="roo.io/absolute-path-bang.html">absolute-path!</a></li>
<li><a href="roo.io/basename.html">basename</a></li>
<li><a href="roo.io/copy-file-bang.html">copy-file!</a></li>
<li><a href="roo.io/current-directory-bang.html">current-directory!</a></li>
<li><a href="roo.io/directory-p.html">directory?</a></li>
<li><a href="roo.io/dirname.html">dirname</a></li>
<li><a href="roo.io/exists-p.html">exists?</a></li>
<li><a href="roo.io/extension.html">extension</a></li>
<li><a href="roo.io/file-p.html">file?</a></li>
<li><a href="roo.io/home-directory-bang.html">home-directory!</a></li>
<li><a href="roo.io/join-path.html">join-path</a></li>
<li><a href="roo.io/list-directory-bang.html">list-directory!</a></li>
<li><a href="roo.io/normalize-path.html">normalize-path</a></li>
<li><a href="roo.io/parent-path.html">parent-path</a></li>
<li><a href="roo.io/read-link-bang.html">read-link!</a></li>
<li><a href="roo.io/remove-tree-bang.html">remove-tree!</a></li>
<li><a href="roo.io/slurp-bang.html">slurp!</a></li>
<li><a href="roo.io/slurp-edn-bang.html">slurp-edn!</a></li>
<li><a href="roo.io/spit-bang.html">spit!</a></li>
<li><a href="roo.io/spit-edn-bang.html">spit-edn!</a></li>
<li><a href="roo.io/stat-bang.html">stat!</a></li>
<li><a href="roo.io/symlink-bang.html">symlink!</a></li>
<li><a href="roo.io/symlink-p.html">symlink?</a></li>
<li><a href="roo.io/walk-bang.html">walk!</a></li>
</ol>
</nav>
<main class="reference-content" markdown="block">

<nav class="reference-breadcrumbs" aria-label="Breadcrumbs">
<a href="../../../../../">Reference</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<a href="../">roo next</a>
<span class="breadcrumb-separator" aria-hidden="true">/</span>
<span aria-current="page">roo.io</span>
</nav>
<div class="version-context">
<span class="version-package">roo</span>
<label class="version-select-label">Version <select class="version-select" onchange="if (this.value) window.location.href = this.value;">
{% assign package_publications = site.data.publications.packages["roo"].publications %}
{% for publication in package_publications %}
<option value="../../{{ publication.identity }}/namespaces/roo.io.html"{% if publication.identity == "next" %} selected{% endif %}>{{ publication.identity }}</option>
{% endfor %}
</select></label>
</div>
# roo.io

| Symbol                                                     | Summary                                                                      |
| ---------------------------------------------------------- | ---------------------------------------------------------------------------- |
| [`absolute-path!`](roo.io/absolute-path-bang.html)         | Resolve a path to an absolute path using the active filesystem.              |
| [`basename`](roo.io/basename.html)                         | Return the filename component of a path string.                              |
| [`copy-file!`](roo.io/copy-file-bang.html)                 | Copy one file, creating parent directories for the destination.              |
| [`current-directory!`](roo.io/current-directory-bang.html) | Return the process current directory according to the active filesystem.     |
| [`directory?`](roo.io/directory-p.html)                    | Query whether a path exists and is a directory.                              |
| [`dirname`](roo.io/dirname.html)                           | Return the directory component of a path string.                             |
| [`exists?`](roo.io/exists-p.html)                          | Query whether any filesystem entry exists at a path.                         |
| [`extension`](roo.io/extension.html)                       | Return the extension component of a path string.                             |
| [`file?`](roo.io/file-p.html)                              | Query whether a path exists and is a regular file.                           |
| [`home-directory!`](roo.io/home-directory-bang.html)       | Return the current user's home directory according to the active filesystem. |
| [`join-path`](roo.io/join-path.html)                       | Join path segments and normalize the result lexically.                       |
| [`list-directory!`](roo.io/list-directory-bang.html)       | Return directory entries for a path, optionally filtered by options.         |
| [`normalize-path`](roo.io/normalize-path.html)             | Normalize a path lexically without accessing the filesystem.                 |
| [`parent-path`](roo.io/parent-path.html)                   | Return the parent path of a path string.                                     |
| [`read-link!`](roo.io/read-link-bang.html)                 | Read the target of a symbolic link.                                          |
| [`remove-tree!`](roo.io/remove-tree-bang.html)             | Remove a file or directory tree if it exists.                                |
| [`slurp!`](roo.io/slurp-bang.html)                         | Read the complete contents of a text file as a string.                       |
| [`slurp-edn!`](roo.io/slurp-edn-bang.html)                 | Read one EDN form from a text file.                                          |
| [`spit!`](roo.io/spit-bang.html)                           | Write a string to a text file.                                               |
| [`spit-edn!`](roo.io/spit-edn-bang.html)                   | Write the string representation of a Roo value to a file as EDN.             |
| [`stat!`](roo.io/stat-bang.html)                           | Return metadata about a filesystem path.                                     |
| [`symlink!`](roo.io/symlink-bang.html)                     | Create a symbolic link.                                                      |
| [`symlink?`](roo.io/symlink-p.html)                        | Test whether a path is a symbolic link.                                      |
| [`walk!`](roo.io/walk-bang.html)                           | Walk a filesystem tree with userspace keep and descent criteria.             |

</main>
</div>
