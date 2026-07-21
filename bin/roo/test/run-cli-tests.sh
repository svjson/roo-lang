#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
PROOF_SMOKE_PACKAGE="$PACKAGE_STAGE_ROOT/proof/test/assets/dynamic-smoke"
RUN_DIR="$ROOT_DIR/build/roo-cli-main-run"
OUTPUT_FILE="$RUN_DIR/main-ran.txt"

cmake -E make_directory "$RUN_DIR"
"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" alpha beta
test "$(cat "$OUTPUT_FILE")" = "3:alpha:beta:nil:nil:nil"

"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" --version --help
test "$(cat "$OUTPUT_FILE")" = "3:--version:--help:nil:nil:nil"

PROOF_OUTPUT="$(
  cd "$PROOF_SMOKE_PACKAGE"
  "$ROO" proof --reporter tree --filter discovered-proof
)"

case "$PROOF_OUTPUT" in
  *"test/smoke/discovered.roo"*"└── PASS - discovered-proof"*) ;;
  *) echo "$PROOF_OUTPUT"; exit 1 ;;
esac

PROOF_HELP_OUTPUT="$(
  cd "$PROOF_SMOKE_PACKAGE"
  "$ROO" proof --help
)"

case "$PROOF_HELP_OUTPUT" in
  *"Usage: roo proof [options]"*"--reporter simple|tree"*) ;;
  *) echo "$PROOF_HELP_OUTPUT"; exit 1 ;;
esac

case "$PROOF_HELP_OUTPUT" in
  *"test/smoke/discovered.roo"*|*"PASS "*|*"FAIL "*|*"ERROR "*) echo "$PROOF_HELP_OUTPUT"; exit 1 ;;
  *) ;;
esac
