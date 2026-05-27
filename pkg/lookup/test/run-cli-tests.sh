#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
LISPLE="$ROOT_DIR/build/lisple"
LOOKUP_PACKAGE="$ROOT_DIR/pkg/lookup"

(
  cd "$ROOT_DIR/pkg/lookup/test"
  "$LISPLE" proof
)

test "$("$LISPLE" "$LOOKUP_PACKAGE" --help)" = "lookup: build Lisple symbol index artifacts
Usage: lookup [--help|--version]
       lookup index [--out <file>] <package-dir>"

test "$("$LISPLE" "$LOOKUP_PACKAGE" --version)" = "lookup 0.1.0"

INDEX_OUTPUT=$("$LISPLE" "$LOOKUP_PACKAGE" index "$LOOKUP_PACKAGE")
test "$INDEX_OUTPUT" = "lookup: index generation is not implemented yet; default output will be lisple-symbols.edn
Usage: lookup [--help|--version]
       lookup index [--out <file>] <package-dir>"
