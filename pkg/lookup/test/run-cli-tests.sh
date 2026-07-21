#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
LOOKUP_PACKAGE="$PACKAGE_STAGE_ROOT/lookup"

(
  cd "$LOOKUP_PACKAGE/test"
  "$ROO" proof
)

test "$("$ROO" "$LOOKUP_PACKAGE" --help)" = "lookup: build Roo symbol index artifacts
Usage: lookup [--help|--version]
       lookup index [--out <file>] <package-dir>
       lookup thing-at <package-dir> <file> <line> <column>"

test "$("$ROO" "$LOOKUP_PACKAGE" --version)" = "lookup 0.1.0"

INDEX_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" index "$LOOKUP_PACKAGE")
case "$INDEX_OUTPUT" in
  *":format :roo/symbol-index"*":package {:name \"lookup\""*":qualified-name \"lookup.cli/command\""*) ;;
  *)
    echo "unexpected lookup index output:" >&2
    echo "$INDEX_OUTPUT" >&2
    exit 1
    ;;
esac

THING_AT_OUTPUT=$(printf '(ns sample.core)\n(let [command-value 1]\n  command-value)\n' | "$ROO" "$LOOKUP_PACKAGE" thing-at . src/sample.roo 3 3)
case "$THING_AT_OUTPUT" in
  *":status :ok"*":text \"command-value\""*":role :local-reference"*":signature \"local binding\""*) ;;
  *)
    echo "unexpected lookup thing-at output:" >&2
    echo "$THING_AT_OUTPUT" >&2
    exit 1
    ;;
esac
