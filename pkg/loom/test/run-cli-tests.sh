#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="$ROOT_DIR/build/roo"
LOOM_PACKAGE="$ROOT_DIR/pkg/loom"
LOOM_REPO="$ROOT_DIR/build/loom-test-repo"
LOOM_LINK_REPO="$ROOT_DIR/build/loom-link-test-repo"
PROOF_REPO="$ROOT_DIR/build/loom-proof-test-repo"

(
  cd "$ROOT_DIR/pkg/loom/test"
  "$ROO" proof
)

cmake -E rm -rf "$LOOM_REPO"
"$ROO" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO" --force

test -f "$LOOM_REPO/loom/0.1.0/package.edn"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/core.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/deps.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/install.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/link.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/uninstall.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/list.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/info.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/args.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/dependency-graph.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/files.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/manifest.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/package-spec.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/common/repository.roo"

test "$("$ROO" "$LOOM_PACKAGE" list --repo "$LOOM_REPO")" = "loom@0.1.0"
"$ROO" "$LOOM_PACKAGE" info loom@0.1.0 --repo "$LOOM_REPO"
DEPS_OUTPUT=$("$ROO" "$LOOM_PACKAGE" deps "$LOOM_PACKAGE" --repo "$LOOM_REPO" --flat)
test "$DEPS_OUTPUT" = ""

cmake -E rm -rf "$LOOM_LINK_REPO"
"$ROO" "$LOOM_PACKAGE" link "$LOOM_PACKAGE" --repo "$LOOM_LINK_REPO"
test -L "$LOOM_LINK_REPO/loom/0.1.0"
test "$("$ROO" "$LOOM_PACKAGE" list --repo "$LOOM_LINK_REPO")" = "loom@0.1.0"
"$ROO" "$LOOM_PACKAGE" uninstall loom@0.1.0 --repo "$LOOM_LINK_REPO"
test ! -e "$LOOM_LINK_REPO/loom/0.1.0"

cmake -E touch "$LOOM_REPO/loom/0.1.0/stale-file"
"$ROO" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO"
test ! -e "$LOOM_REPO/loom/0.1.0/stale-file"

"$ROO" "$LOOM_PACKAGE" uninstall loom@0.1.0 --repo "$LOOM_REPO"
test ! -e "$LOOM_REPO/loom/0.1.0/package.edn"

cmake -E rm -rf "$PROOF_REPO"
"$ROO" "$LOOM_PACKAGE" install "$ROOT_DIR/pkg/proof" --repo "$PROOF_REPO" --force
test -f "$PROOF_REPO/proof/0.1.0/package.edn"
test -f "$PROOF_REPO/proof/0.1.0/native/libproof-native.so"
