# cli-trooper

cli-trooper is a CLI argument parser and dispatcher for Roo.

Enables describing the inputs of a CLI application as data - commands, flags, positionals,
and reusable mixins - and cli-trooper parses `args` against that description and
dispatches to the matching command.

```clojure
(ns app
  (:require [cli-trooper :as cli]))

(defun index-command! [context options]
  (prn (:package-dir options)))

(def config
  {:application "lookup"
   :description "build Roo symbol index artifacts"
   :commands {"index"
              {:execute! index-command!
               :flags {["o"] :path-or-file}
               :positional [{:type :dir :name "package-dir" :optional? true}]}}})

(cli/run config {:args args})
```

`cli-trooper/parse` normalizes the supplied declaration and returns the parsed
command, practical options, and structured errors without invoking the
declared `:execute!` function.

`cli-trooper/run` returns the parsed command, practical options, structured
errors, and the command execution result. `cli-trooper/run!` prints rendered CLI
diagnostics and returns an integer command result as the process status, `0`
for other successful results, or `2` for CLI failures.
