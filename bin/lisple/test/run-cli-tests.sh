#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
LISPLE="$ROOT_DIR/build/lisple"
RUN_DIR="$ROOT_DIR/build/lisple-cli-main-run"
OUTPUT_FILE="$RUN_DIR/main-ran.txt"

cmake -E make_directory "$RUN_DIR"
"$LISPLE" "$ROOT_DIR/bin/lisple/test/assets/main-app" "$OUTPUT_FILE" alpha beta
test "$(cat "$OUTPUT_FILE")" = "3:alpha:beta:nil:nil:nil"
