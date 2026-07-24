# Installing Roo

Roo releases are distributed as platform-specific archives from the GitHub
Release page for each version tag.

Archive names follow this shape:

```text
roo-<version>-linux-x86_64.tar.gz
roo-<version>-macos-arm64.tar.gz
roo-<version>-macos-x86_64.tar.gz
roo-<version>-windows-x86_64.zip
```

Each archive contains a self-contained Roo install tree:

```text
roo-<version>-<platform>/
  bin/
    roo
    rooc
    roo-server
  lib/
  include/
  lib/cmake/
  share/roo/pkg/
  VERSION
  RELEASE.json
  LICENSE
  README.md
```

## Verify The Download

Each release publishes a `SHA256SUMS` file next to the archives.

On Linux:

```sh
sha256sum -c SHA256SUMS
```

On macOS:

```sh
shasum -a 256 -c SHA256SUMS
```

On Windows PowerShell, compare the published checksum with:

```powershell
Get-FileHash .\roo-<version>-windows-x86_64.zip -Algorithm SHA256
```

## Install For Command-Line Use

Unpack the archive somewhere stable.

Linux or macOS:

```sh
tar -xzf roo-<version>-<platform>.tar.gz
export PATH="$PWD/roo-<version>-<platform>/bin:$PATH"
roo --version
rooc --version
```

Windows PowerShell:

```powershell
Expand-Archive .\roo-<version>-windows-x86_64.zip
$env:Path = "$PWD\roo-<version>-windows-x86_64\bin;$env:Path"
roo --version
rooc --version
```

The release binaries are built to find their sibling runtime libraries from the
unpacked tree. Do not set `LD_LIBRARY_PATH` or equivalent variables for normal
use.

## Bundled Packages

The release includes the in-tree Roo packages under:

```text
share/roo/pkg/
```

The first releases bundle these packages so Roo can bootstrap practical package
usage before a Loom package repository exists. Over time, only core packages
such as `loom` and possibly `proof` are expected to remain bundled directly with
Roo.

You can run a bundled package by path:

```sh
roo ./roo-<version>-<platform>/share/roo/pkg/loom --version
roo ./roo-<version>-<platform>/share/roo/pkg/proof --help
```

Packages with native libraries keep those libraries inside their package
directory, under `native/`.

## Building Roo Applications

For Roo-only applications, put the release `bin` directory on `PATH` and call
`roo` or `rooc` from your build scripts.

Example:

```sh
export ROO_ROOT="$PWD/roo-<version>-<platform>"
export PATH="$ROO_ROOT/bin:$PATH"

rooc build path/to/app --build-dir build/app --name app
```

## CMake Consumers

C++ applications that embed Roo can use the CMake package files included in the
archive:

```cmake
find_package(roo REQUIRED)
find_package(roo-package REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE Roo::roo Roo::package_shared)
```

If Roo is unpacked into a non-standard location, pass the release root as
`CMAKE_PREFIX_PATH`:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="$PWD/roo-<version>-<platform>"
cmake --build build
```

This is only needed for CMake to locate Roo's package config files. It is not
needed just to run `roo` or `rooc`.

## GitHub Actions

Downstream projects can pin an exact Roo version by downloading the release
archive in CI.

```yaml
- name: Download Roo
  run: |
    version=0.1.0
    platform=linux-x86_64
    curl -L -o roo.tar.gz \
      "https://github.com/<owner>/<repo>/releases/download/v${version}/roo-${version}-${platform}.tar.gz"
    tar -xzf roo.tar.gz
    echo "$PWD/roo-${version}-${platform}/bin" >> "$GITHUB_PATH"

- name: Build application
  run: |
    rooc build path/to/app --build-dir build/app --name app
```

For CMake consumers, add:

```yaml
env:
  CMAKE_PREFIX_PATH: ${{ github.workspace }}/roo-0.1.0-linux-x86_64
```

