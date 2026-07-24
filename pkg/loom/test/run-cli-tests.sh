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

fail()
{
  printf '%s\n' "loom CLI test failed: $1" >&2
  exit 1
}

assert_file()
{
  test -f "$1" || fail "expected file to exist: $1"
}

assert_symlink()
{
  test -L "$1" || fail "expected symlink to exist: $1"
}

assert_not_exists()
{
  test ! -e "$1" || fail "expected path not to exist: $1"
}

assert_eq()
{
  label=$1
  expected=$(printf '%s' "$2" | tr -d '\r')
  actual=$(printf '%s' "$3" | tr -d '\r')
  if [ "$actual" != "$expected" ]; then
    printf '%s\n' "loom CLI test failed: $label" >&2
    printf '%s\n' "expected:" >&2
    printf '%s\n' "$expected" >&2
    printf '%s\n' "actual:" >&2
    printf '%s\n' "$actual" >&2
    exit 1
  fi
}

manifest_contents()
{
  tr -d '\r' < "$1"
}

expected_init_manifest()
{
  printf '%s\n' \
    "{:name cli-sample" \
    " :version \"0.2.0\"" \
    " :dependencies []" \
    " :load-roots [\"src\"]}"
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

assert_file "$LOOM_REPO/loom/0.1.0/package.edn"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/core.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/deps.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/init.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/install.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/link.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/uninstall.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/list.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/command/info.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/args.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/dependency-graph.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/files.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/manifest.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/package-spec.roo"
assert_file "$LOOM_REPO/loom/0.1.0/src/loom/common/repository.roo"

assert_eq "loom list output" "loom@0.1.0" "$("$ROO" "$LOOM_PACKAGE" list --repo "$LOOM_REPO")"
"$ROO" "$LOOM_PACKAGE" info loom@0.1.0 --repo "$LOOM_REPO"
DEPS_OUTPUT=$("$ROO" "$LOOM_PACKAGE" deps "$LOOM_PACKAGE" --repo "$LOOM_REPO" --flat)
assert_eq "loom deps --flat output" "" "$DEPS_OUTPUT"

printf '%s\n' "==> Testing loom init"
cmake -E rm -rf "$LOOM_INIT_DIR"
"$ROO" "$LOOM_PACKAGE" init "$LOOM_INIT_DIR" --name cli-sample --version 0.2.0
assert_file "$LOOM_INIT_DIR/package.edn"
assert_eq "loom init package.edn contents" \
  "$(expected_init_manifest)" \
  "$(manifest_contents "$LOOM_INIT_DIR/package.edn")"
assert_eq "loom init existing-manifest output" \
  "loom: package manifest already exists: $LOOM_INIT_DIR/package.edn" \
  "$("$ROO" "$LOOM_PACKAGE" init "$LOOM_INIT_DIR" --name overwritten)"
assert_eq "loom init preserves existing package.edn contents" \
  "$(expected_init_manifest)" \
  "$(manifest_contents "$LOOM_INIT_DIR/package.edn")"

printf '%s\n' "==> Testing loom link/uninstall"
cmake -E rm -rf "$LOOM_LINK_REPO"
"$ROO" "$LOOM_PACKAGE" link "$LOOM_PACKAGE" --repo "$LOOM_LINK_REPO"
assert_symlink "$LOOM_LINK_REPO/loom/0.1.0"
assert_eq "loom list linked repo output" \
  "loom@0.1.0" \
  "$("$ROO" "$LOOM_PACKAGE" list --repo "$LOOM_LINK_REPO")"
"$ROO" "$LOOM_PACKAGE" uninstall loom@0.1.0 --repo "$LOOM_LINK_REPO"
assert_not_exists "$LOOM_LINK_REPO/loom/0.1.0"

cmake -E touch "$LOOM_REPO/loom/0.1.0/stale-file"
"$ROO" "$LOOM_PACKAGE" install "$LOOM_PACKAGE" --repo "$LOOM_REPO"
assert_not_exists "$LOOM_REPO/loom/0.1.0/stale-file"

"$ROO" "$LOOM_PACKAGE" uninstall loom@0.1.0 --repo "$LOOM_REPO"
assert_not_exists "$LOOM_REPO/loom/0.1.0/package.edn"

printf '%s\n' "==> Testing loom native package install"
cmake -E rm -rf "$PROOF_REPO"
"$ROO" "$LOOM_PACKAGE" install "$PACKAGE_STAGE_ROOT/proof" --repo "$PROOF_REPO" --force
assert_file "$PROOF_REPO/proof/0.1.0/package.edn"
assert_file "$PROOF_REPO/proof/0.1.0/native/$PROOF_NATIVE_LIBRARY"
