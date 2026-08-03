#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
PROOF_SMOKE_PACKAGE="$PACKAGE_STAGE_ROOT/proof/test/assets/dynamic-smoke"
RUN_DIR="$ROOT_DIR/build/roo-cli-main-run"
OUTPUT_FILE="$RUN_DIR/main-ran.txt"

fail()
{
  printf '%s\n' "roo CLI test failed: $1" >&2
  exit 1
}

assert_eq()
{
  label=$1
  expected=$(printf '%s' "$2" | tr -d '\r')
  actual=$(printf '%s' "$3" | tr -d '\r')
  if [ "$actual" != "$expected" ]; then
    printf '%s\n' "roo CLI test failed: $label" >&2
    printf '%s\n' "expected:" >&2
    printf '%s\n' "$expected" >&2
    printf '%s\n' "actual:" >&2
    printf '%s\n' "$actual" >&2
    exit 1
  fi
}

assert_contains()
{
  label=$1
  haystack=$2
  needle=$3
  case "$haystack" in
    *"$needle"*) ;;
    *)
      printf '%s\n' "roo CLI test failed: $label" >&2
      printf '%s\n' "missing:" >&2
      printf '%s\n' "$needle" >&2
      printf '%s\n' "output:" >&2
      printf '%s\n' "$haystack" >&2
      exit 1
      ;;
  esac
}

assert_not_contains()
{
  label=$1
  haystack=$2
  needle=$3
  case "$haystack" in
    *"$needle"*)
      printf '%s\n' "roo CLI test failed: $label" >&2
      printf '%s\n' "unexpected:" >&2
      printf '%s\n' "$needle" >&2
      printf '%s\n' "output:" >&2
      printf '%s\n' "$haystack" >&2
      exit 1
      ;;
    *) ;;
  esac
}

printf '%s\n' "==> Testing roo main invocation"
cmake -E make_directory "$RUN_DIR"
"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" alpha beta \
  || fail "roo main app invocation failed"
assert_eq "roo main app receives positional args" \
  "3:alpha:beta:nil:nil:nil" \
  "$(cat "$OUTPUT_FILE")"

"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" --version --help \
  || fail "roo main app flag passthrough invocation failed"
assert_eq "roo main app receives flag-like args" \
  "3:--version:--help:nil:nil:nil" \
  "$(cat "$OUTPUT_FILE")"

printf '%s\n' "==> Testing roo proof reporter/filter"
if ! PROOF_OUTPUT="$(
  cd "$PROOF_SMOKE_PACKAGE"
  "$ROO" proof --reporter tree --filter discovered-proof
)"; then
  fail "roo proof reporter/filter command failed"
fi

assert_contains "roo proof output includes discovered file" \
  "$PROOF_OUTPUT" \
  "test/smoke/discovered.roo"
assert_contains "roo proof output includes tree pass" \
  "$PROOF_OUTPUT" \
  "└──"
assert_contains "roo proof output includes pass status" \
  "$PROOF_OUTPUT" \
  "PASS"
assert_contains "roo proof output includes test name" \
  "$PROOF_OUTPUT" \
  "discovered-proof"

printf '%s\n' "==> Testing roo proof help"
if ! PROOF_HELP_OUTPUT="$(
  cd "$PROOF_SMOKE_PACKAGE"
  "$ROO" proof --help
)"; then
  fail "roo proof help command failed"
fi

assert_contains "roo proof help includes usage" \
  "$PROOF_HELP_OUTPUT" \
  "Usage: roo proof [options]"
assert_contains "roo proof help includes reporter option" \
  "$PROOF_HELP_OUTPUT" \
  "--reporter simple|tree"

assert_not_contains "roo proof help does not run tests" "$PROOF_HELP_OUTPUT" "test/smoke/discovered.roo"
assert_not_contains "roo proof help does not report passes" "$PROOF_HELP_OUTPUT" "PASS "
assert_not_contains "roo proof help does not report failures" "$PROOF_HELP_OUTPUT" "FAIL "
assert_not_contains "roo proof help does not report errors" "$PROOF_HELP_OUTPUT" "ERROR "
