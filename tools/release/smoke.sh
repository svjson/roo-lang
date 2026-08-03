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
config=${ROO_RELEASE_CONFIG:-Release}
archive_dir=$(CDPATH= cd -- "$(dirname -- "$archive")" && pwd)
archive="$archive_dir/$(basename -- "$archive")"

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
  MINGW*|MSYS*|CYGWIN*)
    exe_suffix=.exe
    PATH="$release_root/bin:$PATH"
    export PATH
    ;;
esac

roo="$release_root/bin/roo$exe_suffix"
rooc="$release_root/bin/rooc$exe_suffix"

fail()
{
  printf '%s\n' "release smoke failed: $1" >&2
  exit 1
}

assert_eq()
{
  label=$1
  expected=$(printf '%s' "$2" | tr -d '\r')
  actual=$(printf '%s' "$3" | tr -d '\r')
  if [ "$actual" != "$expected" ]; then
    printf '%s\n' "release smoke failed: $label" >&2
    printf '%s\n' "expected:" >&2
    printf '%s\n' "$expected" >&2
    printf '%s\n' "actual:" >&2
    printf '%s\n' "$actual" >&2
    exit 1
  fi
}

host_path()
{
  case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
      if command -v cygpath >/dev/null 2>&1; then
        cygpath -m "$1"
        return
      fi
      ;;
  esac
  printf '%s\n' "$1"
}

printf '%s\n' "==> Testing release Roo language index"
roo_lang_index="$release_root/share/roo/indexes/roo-lang/$version/roo-symbols.edn"
if [ ! -s "$roo_lang_index" ]; then
  fail "missing Roo language index: $roo_lang_index"
fi

printf '%s\n' "==> Testing release tool versions"
assert_eq "roo --version output" "roo $version" "$("$roo" --version)"
assert_eq "rooc --version output" "rooc $version" "$("$rooc" --version)"

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

printf '%s\n' "==> Testing release roo package execution"
assert_eq "roo smoke package output" "smoke-app-ok" "$("$roo" "$smoke_pkg")"

compiled_dir="$tmp_dir/compiled-smoke"
printf '%s\n' "==> Testing release rooc package build"
"$rooc" build "$smoke_pkg" --build-dir "$compiled_dir" --name smoke_app
compiled_app="$compiled_dir/build/smoke_app$exe_suffix"
if [ ! -f "$compiled_app" ]; then
  compiled_app="$compiled_dir/build/$config/smoke_app$exe_suffix"
fi
assert_eq "compiled smoke package output" "smoke-app-ok" "$("$compiled_app")"

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

printf '%s\n' "==> Testing release CMake package"
cmake -S "$cmake_pkg" -B "$cmake_pkg/build" -DCMAKE_PREFIX_PATH="$release_root"
cmake --build "$cmake_pkg/build" --config "$config" --parallel "$jobs"
cmake_smoke="$cmake_pkg/build/roo_release_cmake_smoke$exe_suffix"
if [ ! -f "$cmake_smoke" ]; then
  cmake_smoke="$cmake_pkg/build/$config/roo_release_cmake_smoke$exe_suffix"
fi
"$cmake_smoke"

if [ -d "$release_root/share/roo/pkg/lookup" ]; then
  printf '%s\n' "==> Testing release lookup package"
  assert_eq "lookup --version output" \
    "lookup 0.1.0" \
    "$("$roo" "$release_root/share/roo/pkg/lookup" --version)"
fi

if [ -d "$release_root/share/roo/pkg/proofread" ]; then
  printf '%s\n' "==> Testing release proofread package"
  assert_eq "proofread --version output" \
    "proofread 0.1.0" \
    "$("$roo" "$release_root/share/roo/pkg/proofread" --version)"
fi

footsteps_pkg="$tmp_dir/footsteps-smoke"
footsteps_dependency_path=$(host_path "$release_root/share/roo/pkg/footsteps")
cmake -E make_directory "$footsteps_pkg/src/footsteps_smoke"
cat > "$footsteps_pkg/package.edn" <<EOF
{:name footsteps-smoke
 :version "0.1.0"
 :dependencies {footsteps "file:$footsteps_dependency_path"}
 :load-roots ["src"]
 :main footsteps_smoke.app/main}
EOF
cat > "$footsteps_pkg/src/footsteps_smoke/app.roo" <<'EOF'
(ns footsteps_smoke.app
  (:require [footsteps :as footsteps]))

(defun main [args]
  (let [work (footsteps/make
              {:id :smoke
               :data {}
               :steps {:done {:label "Done"
                              :run (fn [_ctx] {:data {:ok true}})}}
               :start [:done]})
        advanced (footsteps/advance work)]
    (prn (if (= true (:ok (footsteps/data (:work advanced))))
           "footsteps-smoke-ok"
           "footsteps-smoke-fail"))))
EOF

printf '%s\n' "==> Testing release footsteps package"
assert_eq "footsteps smoke package output" \
  "footsteps-smoke-ok" \
  "$("$roo" "$footsteps_pkg")"

printf '%s\n' "release smoke passed: $archive"
