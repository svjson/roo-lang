# boodle

Reference documentation catalog tooling for Roo.

`boodle` converts structured documentation sources into a common catalog model.
The initial supported source format is the lookup symbol index shape.

GitHub Pages output can be rendered to file maps and written to an output
directory:

```roo
(ns docs.build
  (:require [boodle.format.github-pages :as pages]
            [boodle.output.files :as files]))

(files/write! "build/docs" (pages/files catalog))
```
