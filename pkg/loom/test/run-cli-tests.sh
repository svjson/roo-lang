#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
LISPLE="$ROOT_DIR/build/lisple"
LOOM_PACKAGE="$ROOT_DIR/pkg/loom"
LOOM_REPO="$ROOT_DIR/build/loom-test-repo"
PROOF_REPO="$ROOT_DIR/build/loom-proof-test-repo"

(
  cd "$ROOT_DIR/pkg/loom/test"
  "$LISPLE" proof
)

cmake -E rm -rf "$LOOM_REPO"
"$LISPLE" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO" --force

test -f "$LOOM_REPO/loom/0.1.0/package.edn"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/core.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/deps.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/install.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/uninstall.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/list.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/info.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/args.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/dependency-graph.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/files.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/manifest.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/package-spec.lisple"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/repository.lisple"

test "$("$LISPLE" "$LOOM_PACKAGE" list --repo "$LOOM_REPO")" = "loom@0.1.0"
"$LISPLE" "$LOOM_PACKAGE" info loom@0.1.0 --repo "$LOOM_REPO"
DEPS_OUTPUT=$("$LISPLE" "$LOOM_PACKAGE" deps "$LOOM_PACKAGE" --repo "$LOOM_REPO" --flat)
test "$DEPS_OUTPUT" = ""

cmake -E touch "$LOOM_REPO/loom/0.1.0/stale-file"
"$LISPLE" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO"
test ! -e "$LOOM_REPO/loom/0.1.0/stale-file"

"$LISPLE" "$LOOM_PACKAGE" uninstall loom@0.1.0 --repo "$LOOM_REPO"
test ! -e "$LOOM_REPO/loom/0.1.0/package.edn"

cmake -E rm -rf "$PROOF_REPO"
"$LISPLE" "$LOOM_PACKAGE" install "$ROOT_DIR/pkg/proof" --repo "$PROOF_REPO" --force
test -f "$PROOF_REPO/proof/0.1.0/package.edn"
test -f "$PROOF_REPO/proof/0.1.0/src/proof/core.lisple"
test -f "$PROOF_REPO/proof/0.1.0/native/libproof-native.so"
