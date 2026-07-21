#!/bin/sh
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root_dir=$(CDPATH= cd -- "$script_dir/../.." && pwd)
build_dir=${ROO_CI_BUILD_DIR:-"$root_dir/build-platform"}
jobs=${ROO_CI_JOBS:-1}
config=${ROO_CI_CONFIG:-Release}

exe_suffix=
case "$(uname -s)" in
  MINGW*|MSYS*|CYGWIN*) exe_suffix=.exe ;;
esac

find_built_executable()
{
  name=$1
  found=$(find "$build_dir" -type f -name "$name$exe_suffix" | sed -n '1p')
  if [ -z "$found" ]; then
    printf '%s\n' "could not find built executable: $name$exe_suffix" >&2
    exit 1
  fi
  printf '%s\n' "$found"
}

printf '%s\n' "==> Configuring platform test build"
cmake -S "$root_dir" -B "$build_dir" \
  -DCMAKE_BUILD_TYPE="$config" \
  -DROO_SERVER_BUILD_TESTS=ON

printf '%s\n' "==> Building platform test targets"
cmake --build "$build_dir" --config "$config" --parallel "$jobs" \
  --target \
    roo_cli \
    rooc_cli \
    stage_native_packages \
    testroo \
    testpackage \
    testserver \
    testproof \
    testrooc

roo=$(find_built_executable roo)
rooc=$(find_built_executable rooc)
testroo=$(find_built_executable testroo)
testpackage=$(find_built_executable testpackage)
testserver=$(find_built_executable testserver)
testproof=$(find_built_executable testproof)
testrooc=$(find_built_executable testrooc)
stage_root="$build_dir/native-package-stage/pkg"

printf '%s\n' "==> Running C++ test suites"
"$testroo" --skip-benchmarks
"$testpackage"
"$testserver"
"$testproof"
"$testrooc"

printf '%s\n' "==> Running package proof suites"
(
  cd "$stage_root/workbook/test"
  "$roo" proof
)
(
  cd "$stage_root/footsteps/test"
  "$roo" proof
)

printf '%s\n' "==> Running CLI suites"
ROO_BIN="$roo" ROO_PACKAGE_STAGE_ROOT="$stage_root" \
  sh "$root_dir/bin/roo/test/run-cli-tests.sh" "$root_dir"
ROO_BIN="$roo" ROO_PACKAGE_STAGE_ROOT="$stage_root" \
  sh "$root_dir/pkg/loom/test/run-cli-tests.sh" "$root_dir"
ROO_BIN="$roo" ROO_PACKAGE_STAGE_ROOT="$stage_root" \
  sh "$root_dir/pkg/lookup/test/run-cli-tests.sh" "$root_dir"

printf '%s\n' "==> Running CTest CLI and proofread suites"
ctest --test-dir "$build_dir" -C "$config" \
  -R '^(roo_cli_|rooc_cli_|proofread_)' \
  --output-on-failure

printf '%s\n' "==> Building package binaries"
"$rooc" build "$stage_root/loom" --build-dir "$build_dir/loom-build" --name loom
"$rooc" build "$stage_root/lookup" --build-dir "$build_dir/lookup-build" --name lookup
"$rooc" build "$stage_root/proof" --build-dir "$build_dir/proof-build" --name proof
"$rooc" build "$stage_root/proofread" --build-dir "$build_dir/proofread-build" --name proofread

printf '%s\n' "platform tests passed"
