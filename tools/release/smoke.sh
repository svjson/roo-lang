#!/bin/sh
set -eu

if [ "$#" -ne 2 ]; then
  printf '%s\n' "usage: smoke.sh <archive> <version>" >&2
  exit 1
fi

archive=$1
version=$2
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
jobs=${ROO_RELEASE_JOBS:-1}

tmp_dir=$(mktemp -d "${TMPDIR:-/tmp}/roo-release-smoke.XXXXXX")
cleanup()
{
  cmake -E rm -rf "$tmp_dir"
}
trap cleanup EXIT INT TERM

case "$archive" in
  *.zip)
    (
      cd "$tmp_dir"
      cmake -E tar xf "$archive"
    )
    ;;
  *)
    (
      cd "$tmp_dir"
      cmake -E tar xzf "$archive"
    )
    ;;
esac

release_root=$(find "$tmp_dir" -mindepth 1 -maxdepth 1 -type d | sed -n '1p')
if [ -z "$release_root" ]; then
  printf '%s\n' "archive did not contain a release root directory" >&2
  exit 1
fi

exe_suffix=
case "$(uname -s)" in
  MINGW*|MSYS*|CYGWIN*) exe_suffix=.exe ;;
esac

roo="$release_root/bin/roo$exe_suffix"
rooc="$release_root/bin/rooc$exe_suffix"

test "$("$roo" --version)" = "roo $version"
test "$("$rooc" --version)" = "rooc $version"

smoke_pkg="$tmp_dir/smoke-app"
cmake -E make_directory "$smoke_pkg/src/smoke"
cat > "$smoke_pkg/package.edn" <<'EOF'
{:name smoke-app
 :version "0.1.0"
 :load-roots ["src"]
 :main smoke.app/main}
EOF
cat > "$smoke_pkg/src/smoke/app.roo" <<'EOF'
(ns smoke.app)

(defun main [args]
  (prn "smoke-app-ok"))
EOF

test "$("$roo" "$smoke_pkg")" = "smoke-app-ok"

compiled_dir="$tmp_dir/compiled-smoke"
"$rooc" build "$smoke_pkg" --build-dir "$compiled_dir" --name smoke_app
compiled_app="$compiled_dir/build/smoke_app$exe_suffix"
test "$("$compiled_app")" = "smoke-app-ok"

cmake_pkg="$tmp_dir/cmake-smoke"
cmake -E make_directory "$cmake_pkg"
cat > "$cmake_pkg/CMakeLists.txt" <<'EOF'
cmake_minimum_required(VERSION 3.20)
project(roo_release_cmake_smoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(roo REQUIRED)
find_package(roo-package REQUIRED)

add_executable(roo_release_cmake_smoke main.cpp)
target_link_libraries(roo_release_cmake_smoke PRIVATE Roo::roo Roo::package_shared)
EOF
cat > "$cmake_pkg/main.cpp" <<'EOF'
#include <roo/runtime.h>
#include <roo-package/manifest.h>

int main()
{
  Roo::Runtime runtime;
  (void)Roo::Package::default_local_repository_root();
  return runtime.eval("(+ 1 2)")->i32() == 3 ? 0 : 1;
}
EOF

cmake -S "$cmake_pkg" -B "$cmake_pkg/build" -DCMAKE_PREFIX_PATH="$release_root"
cmake --build "$cmake_pkg/build" --parallel "$jobs"
"$cmake_pkg/build/roo_release_cmake_smoke$exe_suffix"

if [ -d "$release_root/share/roo/pkg/lookup" ]; then
  test "$("$roo" "$release_root/share/roo/pkg/lookup" --version)" = "lookup 0.1.0"
fi

if [ -d "$release_root/share/roo/pkg/proofread" ]; then
  test "$("$roo" "$release_root/share/roo/pkg/proofread" --version)" = "proofread 0.1.0"
fi

printf '%s\n' "release smoke passed: $archive"
