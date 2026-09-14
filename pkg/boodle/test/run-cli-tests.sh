#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/package-stage/pkg}"
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

assert_file()
{
  path=$1
  label=$2
  if [ ! -s "$path" ]; then
    rm -rf "$ROOT"
    fail "boodle generate did not write $label"
  fi
}

printf '%s\n' "==> Testing boodle help/version"
if ! HELP_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" --help); then
  fail "boodle --help command failed"
fi
assert_eq "boodle --help output" \
  "boodle: generate reference documentation

Usage: boodle generate <index-file>... [-f,--format <github-pages>] [--navigation <breadcrumbs|up|none>] [--package-group <title:id[,id]>]... --publication-mode <development|prerelease|stable> --publication-identity <identity> -o,--out <dir>" \
  "$HELP_OUTPUT"

if ! VERSION_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" --version); then
  fail "boodle --version command failed"
fi
assert_eq "boodle --version output" \
  "boodle 0.1.0" \
  "$VERSION_OUTPUT"

if INVALID_OUTPUT=$(
  "$ROO" "$BOODLE_PACKAGE" generate --format html 2>&1
); then
  fail "invalid boodle invocation succeeded"
else
  INVALID_STATUS=$?
fi
assert_eq "boodle invalid invocation exit status" \
  "2" \
  "$INVALID_STATUS"
assert_eq "boodle invalid invocation diagnostics" \
  "Invalid value for --format: html. Expected: github-pages.
Missing required option: --publication-mode.
Missing required option: --publication-identity.
Missing required option: --out.
Missing required argument: index-file." \
  "$INVALID_OUTPUT"

printf '%s\n' "==> Testing boodle generate"
ROOT="/tmp/boodle-cli-$$"
INDEX_FILE="$ROOT/roo.edn"
OUTPUT_DIR="$ROOT/docs"
OUTPUT_DISPLAY_DIR="$OUTPUT_DIR"
case "$(uname -s)" in
  MINGW*|MSYS*|CYGWIN*)
    if command -v cygpath >/dev/null 2>&1; then
      OUTPUT_DISPLAY_DIR=$(cygpath -m "$OUTPUT_DIR")
    fi
    ;;
esac
rm -rf "$ROOT"
mkdir -p "$ROOT"
cat > "$INDEX_FILE" <<'EOF'
{:format :roo/symbol-index
 :version 1
 :package {:name "roo" :version "0.1.0" :description "Roo standard library."}
 :namespaces [{:name "roo" :summary "Implicit runtime namespace." :content [{:kind :text :content "Implicit runtime namespace."}]}]
 :symbols [{:id "roo/+" :name "+" :qualified-name "roo/+" :namespace "roo" :kind :function :origin :native :summary "Add numbers." :content [{:kind :text :content "Add numbers."} {:kind :sig-args :index 0} {:kind :sig-ret :index 0}] :signatures [{:display "(+ numbers...)" :params [{:name "numbers" :display "numbers..." :content [{:kind :text :content "Numbers to add."}]}] :returns {:content [{:kind :text :content "The sum."}]}}]}]}
EOF

if ! GENERATE_OUTPUT=$("$ROO" "$BOODLE_PACKAGE" generate -o "$OUTPUT_DIR" --publication-mode development --publication-identity next --package-group "The Roo Language:roo" "$INDEX_FILE"); then
  rm -rf "$ROOT"
  fail "boodle generate command failed"
fi
case "$GENERATE_OUTPUT" in
  "boodle: wrote "*" files to $OUTPUT_DISPLAY_DIR") ;;
  *)
    printf '%s\n' "unexpected boodle generate output:" >&2
    printf '%s\n' "$GENERATE_OUTPUT" >&2
    fail "boodle generate output"
    ;;
esac

for generated_file in \
  "_config.yml:config" \
  "index.md:index" \
  "_layouts/reference.html:reference layout" \
  "assets/boodle.css:stylesheet" \
  "packages/roo/versions/next/index.md:package page" \
  "packages/roo/versions/next/namespaces/roo/plus.md:symbol page"
do
  path=${generated_file%%:*}
  label=${generated_file#*:}
  assert_file "$OUTPUT_DIR/$path" "$label"
done

rm -rf "$ROOT"
