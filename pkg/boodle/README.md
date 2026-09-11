# boodle

Reference documentation catalog tooling for Roo.

`boodle` converts structured documentation sources into a common catalog model.
The initial supported source format is the lookup symbol index shape.

Generate GitHub Pages Markdown from one or more lookup index files:

```sh
boodle generate --format github-pages \
  --publication-mode development \
  --publication-identity next \
  --out build/docs \
  package-symbols.edn
```

Generated hierarchy navigation defaults to breadcrumbs. Use `--navigation up`
for a single parent link, or `--navigation none` to omit hierarchy navigation.

Every generation declares its publication role explicitly:

- `--publication-mode development --publication-identity next` writes the
  mutable development corpus without using package-declared versions as public
  paths.
- `--publication-mode prerelease --publication-identity <version>` accepts a
  SemVer prerelease and replaces the previously retained prerelease.
- `--publication-mode stable --publication-identity <version>` accepts a stable
  SemVer release and retains earlier stable releases.

Stable releases are selected automatically ahead of a retained prerelease.
When only `next` exists, package entrypoints remain neutral and link to the
development documentation explicitly.

Root package listings can be grouped with repeatable `--package-group` options.
Use `*` to include packages not listed by any exact group:

```sh
boodle generate --format github-pages --out build/docs \
  --publication-mode development \
  --publication-identity next \
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
