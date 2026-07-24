#!/usr/bin/env sh
set -eu

ROOT_DIR="${1:?repo root required}"
ROO="${ROO_BIN:-$ROOT_DIR/build/roo}"
PACKAGE_STAGE_ROOT="${ROO_PACKAGE_STAGE_ROOT:-$ROOT_DIR/build/native-package-stage/pkg}"
LOOM_PACKAGE="$PACKAGE_STAGE_ROOT/loom"
LOOM_REPO="$ROOT_DIR/build/loom-test-repo"
LOOM_LINK_REPO="$ROOT_DIR/build/loom-link-test-repo"
LOOM_INIT_DIR="$ROOT_DIR/build/loom-init-test-package"
PROOF_REPO="$ROOT_DIR/build/loom-proof-test-repo"
PROOF_NATIVE_LIBRARY=libproof-native.so

manifest_contents()
{
  tr -d '\r' < "$1"
}

case "$(uname -s)" in
  Darwin) PROOF_NATIVE_LIBRARY=libproof-native.dylib ;;
  MINGW*|MSYS*|CYGWIN*) PROOF_NATIVE_LIBRARY=proof-native.dll ;;
esac

(
  cd "$LOOM_PACKAGE/test"
  "$ROO" proof
)

printf '%s\n' "==> Testing loom install/list/info/deps"
cmake -E rm -rf "$LOOM_REPO"
"$ROO" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO" --force

test -f "$LOOM_REPO/loom/0.1.0/package.edn"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/core.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/deps.roo"
test -f "$LOOM_REPO/loom/0.1.0/src/loom/command/init.roo"
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

printf '%s\n' "==> Testing loom init"
cmake -E rm -rf "$LOOM_INIT_DIR"
"$ROO" "$LOOM_PACKAGE" init "$LOOM_INIT_DIR" --name cli-sample --version 0.2.0
test -f "$LOOM_INIT_DIR/package.edn"
test "$(manifest_contents "$LOOM_INIT_DIR/package.edn")" = "{:name cli-sample
 :version \"0.2.0\"
 :dependencies []
 :load-roots [\"src\"]}"
test "$("$ROO" "$LOOM_PACKAGE" init "$LOOM_INIT_DIR" --name overwritten)" = "loom: package manifest already exists: $LOOM_INIT_DIR/package.edn"
test "$(manifest_contents "$LOOM_INIT_DIR/package.edn")" = "{:name cli-sample
 :version \"0.2.0\"
 :dependencies []
 :load-roots [\"src\"]}"

printf '%s\n' "==> Testing loom link/uninstall"
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

printf '%s\n' "==> Testing loom native package install"
cmake -E rm -rf "$PROOF_REPO"
"$ROO" "$LOOM_PACKAGE" install "$PACKAGE_STAGE_ROOT/proof" --repo "$PROOF_REPO" --force
test -f "$PROOF_REPO/proof/0.1.0/package.edn"
test -f "$PROOF_REPO/proof/0.1.0/native/$PROOF_NATIVE_LIBRARY"
