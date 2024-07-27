# sin-cpp

`sin-cpp` is a basic `C++20` library.

# Build

## Dependencies

Mandatory dependencies:
- `C++20` compiler (for compiling)
- `CMake` (for building)
- [benchmark](https://github.com/google/benchmark) (for benchmarks)
- [gtest](https://github.com/google/googletest) (for tests)
- [zlib](https://github.com/madler/zlib) (for gzip support)
- [sin-cmake](https://github.com/sin-is-nsimd/sin-cmake) (for building)

Optional dependencies:
- `ninja` (for faster building)
- `ccache` (for faster compiling)
- `doxygen` (for documentation)

## For Debian GNU/Linux

You can install theses packages (with `root` access):
```sh
apt install \
  build-essential cmake \
  ninja-build ccache doxygen
```

## For MacOS

TODO

### For Microsoft Windows

`C++` compiler, `CMake` and `ninja` are available in the latest versions of
[Visual Studio](https://visualstudio.microsoft.com/vs/community/).

The commands given for building in the next sections work in a `PowerShell`
shell after theses commands (you may change the `Visual Studio` path):
```sh
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86

set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\VC\SecurityIssueAnalysis\python;%PATH%
```

## Clone `sin-cpp` and Other Dependencies

```sh
git clone https://github.com/google/benchmark.git
git clone https://github.com/google/googletest.git
git clone https://github.com/madler/zlib.git
git clone https://github.com/sin-is-nsimd/sin-cmake.git
git clone https://github.com/sin-is-nsimd/sin-cpp.git
```

## Build `gtest`, `benchmark` and `zlib`

Build `benchmark` (in the `benchmark` directory):
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -GNinja -DCMAKE_INSTALL_PREFIX=../../_install_sincpp
ninja
ninja install
```

Build `gtest` (in the `googletest` directory):
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_INSTALL_PREFIX=../../_install_sincpp
ninja
ninja install
```

Build `zlib` (in the `zlib` directory):
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_INSTALL_PREFIX=../../_install_sincpp
ninja
ninja install
```

## Build `sin-cpp`

Build (in the `sin-cpp` directory):
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_INSTALL_PREFIX=../../_install_sincpp
ninja
ninja install
```

Under `Microsoft Windows`, you may add `-DSINCPP_LIB_TYPE=STATIC -DCMAKE_CXX_FLAGS_RELEASE="/MT"` arguments to the `cmake` command.

With `GCC`-like compilers, you can add this option to the `cmake ..` command:
`-DCMAKE_CXX_FLAGS="-fdiagnostics-color=always -fconcepts-diagnostics-depth=8"`.

# CI

| **Operating System**           | **Compiler**  |  `amd64`       | `i386`         | `armhf`        | `arm64`        | `ppc64el`      |
| ------------------------------ | ------------  | -------------  | -------------  | -------------  | -------------  | -------------- |
| Debian GNU/Linux 12 "Bookworm" | `clang` `gcc` | :green_circle: | :green_circle: | :green_circle: | :green_circle: | :green_circle: |
| Debian GNU/Linux 13 "Trixie"   | `clang` `gcc` | :green_circle: | :green_circle: | :green_circle: | :green_circle: | :green_circle: |
| Raspberry Pi OS 12             | `clang` `gcc` | :white_circle: | :white_circle: | :green_circle: | :green_circle: | :white_circle: |
| Manjaro                        | `clang` `gcc` | :white_circle: | :white_circle: | :black_circle: | :green_circle: | :white_circle: |
| macOS 14                       | `clang`       | :black_circle: | :white_circle: | :white_circle: | :green_circle: | :white_circle: |
| Microsoft Windows 10           | `msvc 2022`   | :green_circle: | :black_circle: | :black_circle: | :black_circle: | :white_circle: |
| Microsoft Windows 11           | `msvc 2022`   | :green_circle: | :black_circle: | :black_circle: | :black_circle: | :white_circle: |

**Legend**:  
:green_circle: Ok / :yellow_circle: In progress / :orange_circle: Stalled / :red_circle: Nok / :white_circle: Not applicable / :black_circle: Not tested

# Development

Project uses `clang-format` as `C++` code formatter.

Project uses `black` as `Python` code formatter.

Project uses [conventional-pre-commit](https://github.com/compilerla/conventional-pre-commit)
to try to force good commit messages.
The `.pre-commit-config.yaml` is already in the root folder of `sin-cmake`.
Run this command in the `sin-cpp` directory:
```sh
pre-commit install --hook-type commit-msg
```

# License

Copyright © 2024 <https://github.com/sin-is-nsimd>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
