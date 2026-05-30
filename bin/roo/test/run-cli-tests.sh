#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="$ROOT_DIR/build/roo"
RUN_DIR="$ROOT_DIR/build/roo-cli-main-run"
OUTPUT_FILE="$RUN_DIR/main-ran.txt"

cmake -E make_directory "$RUN_DIR"
"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" alpha beta
test "$(cat "$OUTPUT_FILE")" = "3:alpha:beta:nil:nil:nil"

"$ROO" "$ROOT_DIR/bin/roo/test/assets/main-app" "$OUTPUT_FILE" --version --help
test "$(cat "$OUTPUT_FILE")" = "3:--version:--help:nil:nil:nil"
