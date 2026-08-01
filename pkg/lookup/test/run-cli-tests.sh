#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
LOOKUP_PACKAGE="$PACKAGE_STAGE_ROOT/lookup"

fail()
{
  printf '%s\n' "lookup CLI test failed: $1" >&2
  exit 1
}

assert_eq()
{
  label=$1
  expected=$(printf '%s' "$2" | tr -d '\r')
  actual=$(printf '%s' "$3" | tr -d '\r')
  if [ "$actual" != "$expected" ]; then
    printf '%s\n' "lookup CLI test failed: $label" >&2
    printf '%s\n' "expected:" >&2
    printf '%s\n' "$expected" >&2
    printf '%s\n' "actual:" >&2
    printf '%s\n' "$actual" >&2
    exit 1
  fi
}

printf '%s\n' "==> Testing lookup proof suite"
(
  cd "$LOOKUP_PACKAGE/test"
  "$ROO" proof
)

printf '%s\n' "==> Testing lookup help/version"
if ! HELP_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" --help); then
  fail "lookup --help command failed"
fi
assert_eq "lookup --help output" \
  "lookup: build Roo symbol index artifacts
Usage: lookup [--help|--version]
       lookup index [-x extractor]... [--root <dir>]... [--exclude <path>]... [-o <file>] [<package-dir>]
       lookup thing-at <package-dir> <file> <line> <column>

Extractors: forms, symbols, native
Default: symbols for packages, native for roots
Roots are scanned by applicable extractors; excludes skip paths and subtrees" \
  "$HELP_OUTPUT"

if ! VERSION_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" --version); then
  fail "lookup --version command failed"
fi
assert_eq "lookup --version output" \
  "lookup 0.1.0" \
  "$VERSION_OUTPUT"

printf '%s\n' "==> Testing lookup index"
if ! INDEX_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" index "$LOOKUP_PACKAGE"); then
  fail "lookup index command failed"
fi
case "$INDEX_OUTPUT" in
  *":format :roo/symbol-index"*":package {:name \"lookup\""*":qualified-name \"lookup.cli/command\""*) ;;
  *)
    printf '%s\n' "unexpected lookup index output:" >&2
    printf '%s\n' "$INDEX_OUTPUT" >&2
    fail "lookup index output"
    ;;
esac

if ! FORMS_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" index -x forms "$LOOKUP_PACKAGE"); then
  fail "lookup index -x forms command failed"
fi
case "$FORMS_OUTPUT" in
  *":format :roo/source-forms"*":package {:name \"lookup\""*":sources ["*) ;;
  *)
    printf '%s\n' "unexpected lookup forms index output:" >&2
    printf '%s\n' "$FORMS_OUTPUT" >&2
    fail "lookup index -x forms output"
    ;;
esac

OUT_FILE="/tmp/lookup-index-output-$$.edn"
rm -f "$OUT_FILE"
if ! "$ROO" "$LOOKUP_PACKAGE" index -o "$OUT_FILE" "$LOOKUP_PACKAGE"; then
  fail "lookup index -o command failed"
fi
if [ ! -s "$OUT_FILE" ]; then
  fail "lookup index -o did not write output"
fi
OUT_CONTENT=$(cat "$OUT_FILE")
rm -f "$OUT_FILE"
case "$OUT_CONTENT" in
  *":format :roo/symbol-index"*":package {:name \"lookup\""*) ;;
  *)
    printf '%s\n' "unexpected lookup index -o file output:" >&2
    printf '%s\n' "$OUT_CONTENT" >&2
    fail "lookup index -o output"
    ;;
esac

if ! NATIVE_ROOT_OUTPUT=$("$ROO" "$LOOKUP_PACKAGE" index --root "$LOOKUP_PACKAGE"/test/assets --exclude "$LOOKUP_PACKAGE"/test/assets/native-package); then
  fail "lookup index --root command failed"
fi
case "$NATIVE_ROOT_OUTPUT" in
  *":origin {:kind :roots"*":excludes ["*":qualified-name \"extra.native/read!\""*":summary \"Read data from an external native source root.\""*) ;;
  *)
    printf '%s\n' "unexpected lookup index --root output:" >&2
    printf '%s\n' "$NATIVE_ROOT_OUTPUT" >&2
    fail "lookup index --root output"
    ;;
esac
case "$NATIVE_ROOT_OUTPUT" in
  *"sample.native/read!"*)
    printf '%s\n' "excluded native source appeared in lookup index --root output:" >&2
    printf '%s\n' "$NATIVE_ROOT_OUTPUT" >&2
    fail "lookup index --root exclude output"
    ;;
esac

printf '%s\n' "==> Testing lookup thing-at"
if ! THING_AT_OUTPUT=$(printf '(ns sample.core)\n(let [command-value 1]\n  command-value)\n' | "$ROO" "$LOOKUP_PACKAGE" thing-at . src/sample.roo 3 3); then
  fail "lookup thing-at command failed"
fi
case "$THING_AT_OUTPUT" in
  *":status :ok"*":text \"command-value\""*":role :local-reference"*":signature \"local binding\""*) ;;
  *)
    printf '%s\n' "unexpected lookup thing-at output:" >&2
    printf '%s\n' "$THING_AT_OUTPUT" >&2
    fail "lookup thing-at output"
    ;;
esac
