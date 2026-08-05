# boodle

Reference documentation catalog tooling for Roo.

`boodle` converts structured documentation sources into a common catalog model.
The initial supported source format is the lookup symbol index shape.

Generate GitHub Pages Markdown from one or more lookup index files:

```sh
boodle generate --format github-pages --out build/docs package-symbols.edn
```

Root package listings can be grouped with repeatable `--package-group` options.
Use `*` to include packages not listed by any exact group:

```sh
boodle generate --format github-pages --out build/docs \
  --package-group "Runtime:core" \
  --package-group "Packages:*" \
  core-symbols.edn app-symbols.edn
```

Preview the generated pages with Jekyll:

```sh
jekyll serve --source build/docs --destination build/docs-site
```

GitHub Pages output can be rendered to file maps and written to an output
directory:

```roo
(ns docs.build
  (:require [boodle.format.github-pages :as pages]
            [boodle.output.files :as files]))

(files/write! "build/docs" (pages/files catalog))
```
