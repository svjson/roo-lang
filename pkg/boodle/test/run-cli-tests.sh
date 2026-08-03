#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
BOODLE_PACKAGE="$PACKAGE_STAGE_ROOT/boodle"

fail()
{
  printf '%s\n' "boodle CLI test failed: $1" >&2
  exit 1
}

assert_eq()
{
  label=$1
  expected=$(printf '%s' "$2" | tr -d '\r')
  actual=$(printf '%s' "$3" | tr -d '\r')
  if [ "$actual" != "$expected" ]; then
    printf '%s\n' "boodle CLI test failed: $label" >&2
    printf '%s\n' "expected:" >&2
    printf '%s\n' "$expected" >&2
    printf '%s\n' "actual:" >&2
    printf '%s\n' "$actual" >&2
    exit 1
  fi
}

printf '%s\n' "==> Testing boodle proof suite"
(
  cd "$BOODLE_PACKAGE/test"
  "$ROO" proof
)

printf '%s\n' "==> Testing boodle help/version"
if ! HELP_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" --help); then
  fail "boodle --help command failed"
fi
assert_eq "boodle --help output" \
  "boodle: generate reference documentation
Usage: boodle [--help|--version]
       boodle generate [-f|--format github-pages] -o|--out <dir> <index-file>...

Formats: github-pages" \
  "$HELP_OUTPUT"

if ! VERSION_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" --version); then
  fail "boodle --version command failed"
fi
assert_eq "boodle --version output" \
  "boodle 0.1.0" \
  "$VERSION_OUTPUT"

printf '%s\n' "==> Testing boodle generate"
ROOT="/tmp/boodle-cli-$$"
INDEX_FILE="$ROOT/roo.edn"
OUTPUT_DIR="$ROOT/docs"
rm -rf "$ROOT"
mkdir -p "$ROOT"
cat > "$INDEX_FILE" <<'EOF'
{:format :roo/symbol-index
 :version 1
 :package {:name "roo" :version "0.1.0" :description "Roo standard library."}
 :namespaces [{:name "roo" :doc {:summary "Implicit runtime namespace."}}]
 :symbols [{:id "roo/+" :name "+" :qualified-name "roo/+" :namespace "roo" :kind :function :origin :native :doc {:summary "Add numbers."} :signatures [{:display "(+ numbers...)" :params [{:name "numbers..." :doc "Numbers to add."}] :returns {:doc "The sum."}}]}]}
EOF

if ! GENERATE_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" generate -o "$OUTPUT_DIR" "$INDEX_FILE"); then
  rm -rf "$ROOT"
  fail "boodle generate command failed"
fi
assert_eq "boodle generate output" \
  "boodle: wrote 4 files to $OUTPUT_DIR" \
  "$GENERATE_OUTPUT"

if [ ! -s "$OUTPUT_DIR/_config.yml" ]; then
  rm -rf "$ROOT"
  fail "boodle generate did not write _config.yml"
fi
if [ ! -s "$OUTPUT_DIR/index.md" ]; then
  rm -rf "$ROOT"
  fail "boodle generate did not write index.md"
fi
if [ ! -s "$OUTPUT_DIR/packages/roo/index.md" ]; then
  rm -rf "$ROOT"
  fail "boodle generate did not write package page"
fi
if [ ! -s "$OUTPUT_DIR/packages/roo/namespaces/roo.md" ]; then
  rm -rf "$ROOT"
  fail "boodle generate did not write namespace page"
fi

NAMESPACE_CONTENT=$(cat "$OUTPUT_DIR/packages/roo/namespaces/roo.md")
rm -rf "$ROOT"
case "$NAMESPACE_CONTENT" in
  *"---"*\
*"title: roo"*\
*"# roo"*\
*"### roo/+"*\
*"| numbers... | Numbers to add. |"*\
*"Returns: The sum."*) ;;
  *)
    printf '%s\n' "unexpected boodle namespace page:" >&2
    printf '%s\n' "$NAMESPACE_CONTENT" >&2
    fail "boodle generate namespace page"
    ;;
esac
