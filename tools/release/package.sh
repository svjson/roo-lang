#!/bin/sh
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root_dir=$(CDPATH= cd -- "$script_dir/../.." && pwd)

version=${1:-${VERSION:-}}
if [ -z "$version" ]; then
  version=$(sed -n '1p' "$root_dir/VERSION" | tr -d '[:space:]')
fi

project_version=$(sed -n '1p' "$root_dir/VERSION" | tr -d '[:space:]')
if [ "$version" != "$project_version" ]; then
  printf '%s\n' "release version '$version' does not match VERSION '$project_version'" >&2
  exit 1
fi

os_name=$(uname -s)
arch_name=$(uname -m)

case "$os_name" in
  Linux) platform_os=linux ;;
  Darwin) platform_os=macos ;;
  MINGW*|MSYS*|CYGWIN*) platform_os=windows ;;
  *)
    printf '%s\n' "unsupported release OS: $os_name" >&2
    exit 1
    ;;
esac

case "$arch_name" in
  x86_64|amd64) platform_arch=x86_64 ;;
  arm64|aarch64) platform_arch=arm64 ;;
  *)
    printf '%s\n' "unsupported release architecture: $arch_name" >&2
    exit 1
    ;;
esac

platform="$platform_os-$platform_arch"
dist_name="roo-$version-$platform"
build_dir=${ROO_RELEASE_BUILD_DIR:-"$root_dir/build-release"}
dist_dir=${ROO_RELEASE_DIST_DIR:-"$root_dir/dist"}
jobs=${ROO_RELEASE_JOBS:-1}
stage_dir="$dist_dir/$dist_name"

if [ "$platform_os" = "windows" ]; then
  archive="$dist_dir/$dist_name.zip"
else
  archive="$dist_dir/$dist_name.tar.gz"
fi

printf '%s\n' "==> Configuring release build"
cmake -S "$root_dir" -B "$build_dir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$stage_dir" \
  -DROO_SERVER_BUILD_TESTS=OFF

printf '%s\n' "==> Building release tree"
cmake --build "$build_dir" --parallel "$jobs"

printf '%s\n' "==> Recreating staged install tree"
cmake -E rm -rf "$stage_dir"
cmake -E make_directory "$stage_dir"
cmake --install "$build_dir"

printf '%s\n' "==> Writing release metadata"
cmake -E copy_if_different "$root_dir/LICENSE" "$stage_dir/LICENSE"
cmake -E copy_if_different "$root_dir/README.md" "$stage_dir/README.md"
printf '%s\n' "$version" > "$stage_dir/VERSION"

revision=unknown
if command -v git >/dev/null 2>&1; then
  revision=$(git -C "$root_dir" rev-parse --short=12 HEAD 2>/dev/null || printf '%s' unknown)
fi

built_at=$(date -u '+%Y-%m-%dT%H:%M:%SZ')
cat > "$stage_dir/RELEASE.json" <<EOF
{
  "name": "roo",
  "version": "$version",
  "platform": "$platform",
  "source_revision": "$revision",
  "built_at": "$built_at"
}
EOF

printf '%s\n' "==> Packing $archive"
cmake -E rm -f "$archive"
(
  cd "$dist_dir"
  if [ "$platform_os" = "windows" ]; then
    cmake -E tar cf "$archive" --format=zip "$dist_name"
  else
    cmake -E tar czf "$archive" "$dist_name"
  fi
)

printf '%s\n' "==> Generating checksums"
(
  cd "$dist_dir"
  if command -v sha256sum >/dev/null 2>&1; then
    sha256sum "$(basename "$archive")" > SHA256SUMS
  elif command -v shasum >/dev/null 2>&1; then
    shasum -a 256 "$(basename "$archive")" > SHA256SUMS
  else
    printf '%s\n' "no SHA256 command found" >&2
    exit 1
  fi
)

printf '%s\n' "==> Smoke testing archive"
sh "$script_dir/smoke.sh" "$archive" "$version"

printf '%s\n' "Release archive: $archive"
printf '%s\n' "Checksums: $dist_dir/SHA256SUMS"
