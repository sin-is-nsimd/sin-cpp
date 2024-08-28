#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Generate github action workflows."""

# Copyright © 2023-2024 Lénaïc Bagnères, lenaicb@singularity.fr
# Copyright © 2024 Rodolphe Cargnello, rodolphe.cargnello@gmail.com

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import sys

config = {}

# debian + raspios + manjaro
for system in [
    # debian12
    "debian12-amd64-sse42",
    "debian12-i386-sse2",
    "debian12-armhf-neon",
    "debian12-arm64-asimd",
    "debian12-ppc64el-vsx",
    # debian13
    "debian13-amd64-sse42",
    "debian13-i386-sse2",
    "debian13-armhf-neon",
    "debian13-arm64-asimd",
    "debian13-ppc64el-vsx",
    # raspios12
    "raspios12-armhf-neon",
    "raspios12-arm64-asimd",
    # manjaro
    "manjaro-arm64-asimd",
]:
    config.update(
        {
            system: {
                "compilers_args": [
                    {
                        "name": "clang",
                        "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang",
                    },
                    {
                        "name": "gcc",
                        "args": "-DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc",
                    },
                ],
                "build_system": [
                    {"name": "ninja", "args": "-GNinja"},
                    {"name": "make", "args": '-G"Unix Makefiles"'},
                ],
            },
        }
    )

# macOS
for system in ["macos13-amd64-avx2", "macos14-amd64-avx2"]:
    config.update(
        {
            system: {
                "compilers_args": [
                    {
                        "name": "clang",
                        "args": '-DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang LDFLAGS="-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -lunwind" CPPFLAGS=-I/usr/local/opt/llvm/include',
                    },
                    {
                        "name": "gcc13",
                        "args": "-DCMAKE_CXX_COMPILER=/usr/local/bin/g++-13 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-13",
                    },
                    {
                        "name": "gcc14",
                        "args": "-DCMAKE_CXX_COMPILER=/usr/local/bin/g++-14 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-14",
                    },
                ],
                "build_system": [
                    {"name": "ninja", "args": "-GNinja"},
                    {"name": "make", "args": '-G"Unix Makefiles"'},
                ],
            },
        }
    )
config["macos14-amd64-avx2"]["compilers_args"] += [
    {
        "name": "apple-clang",
        "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang",
    }
]
config.update(
    {
        "macos14-arm64-asimd": {
            "compilers_args": [
                {
                    "name": "apple-clang",
                    "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang",
                },
                {
                    "name": "clang",
                    "args": '-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang LDFLAGS="-L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++" CPPFLAGS="-I/opt/homebrew/opt/llvm/include"',
                },
                {
                    "name": "gcc13",
                    "args": '-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/gcc@13/bin/g++-13 -DCMAKE_C_COMPILER=/opt/homebrew/opt/gcc@13/bin/gcc-13 LDFLAGS="-L/opt/homebrew/opt/gcc@13/lib -Wl,-rpath,/opt/homebrew/opt/gcc@13/lib" CPPFLAGS="-I/opt/homebrew/opt/gcc@13/include"',
                },
                {
                    "name": "gcc14",
                    "args": '-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/gcc@14/bin/g++-14 -DCMAKE_C_COMPILER=/opt/homebrew/opt/gcc@14/bin/gcc-14 LDFLAGS="-L/opt/homebrew/opt/gcc@14/lib -Wl,-rpath,/opt/homebrew/opt/gcc@14/lib" CPPFLAGS="-I/opt/homebrew/opt/gcc@14/include"',
                },
            ],
            "build_system": [
                {"name": "ninja", "args": "-GNinja"},
                {"name": "make", "args": '-G"Unix Makefiles"'},
            ],
        },
    }
)

# Windows
for system in ["windows10-amd64-sse42", "windows10-i386-sse2", "windows11-amd64-sse42"]:
    config.update(
        {
            system: {
                "compilers_args": [
                    {
                        "name": "msvc",
                        "args": "",
                    },
                ],
                "build_system": [
                    {"name": "ninja", "args": "-GNinja"},
                ],
            },
        },
    )


def get_vcvars_paths(system):
    if system == "windows10-i386-sse2":
        return "C:\\Program Files\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat"
    else:
        return "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"


header = """# THIS FILE IS GENERATED!

name: ci

on:
  # push:
  #   branches: [ main ]
  pull_request:

jobs:
"""

runner = """
  {runner}:
    continue-on-error: true
    runs-on: [ self-hosted, {label} ]
    name: {runner}
    steps:
"""

step_clean_unix = """
      # Clean
      - name: Clean
        run: rm -rf *
"""

step_clean_windows = """
      # Clean
      - name: Clean
        run: rd -r *
"""

step_clone = """
      # Clone
      - name: Clone Google Benchmark
        uses: actions/checkout@v3
        with:
          repository: "google/benchmark"
          path: 'benchmark'
      - name: Clone Google Test
        uses: actions/checkout@v3
        with:
          repository: "google/googletest"
          path: 'googletest'
      - name: Clone zlib
        uses: actions/checkout@v3
        with:
          repository: "madler/zlib"
          path: 'zlib'
      - name: Clone sin-python
        uses: actions/checkout@v3
        with:
          repository: 'sin-is-nsimd/sin-python'
          path: 'sin-python'
      - name: Clone sin-cmake
        uses: actions/checkout@v3
        with:
          repository: 'sin-is-nsimd/sin-cmake'
          path: 'sin-cmake'
      - name: Clone sin-cpp
        uses: actions/checkout@v3
        with:
          repository: 'sin-is-nsimd/sin-cpp'
          path: 'sin-cpp'
"""

step_unix = """
      # Dependencies
      - name: Google Benchmark
        shell: bash
        run: |
          cd benchmark
          cmake \\
            -B build \\
            -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
            -GNinja \\
            -DCMAKE_INSTALL_PREFIX=../_install \\
            {compilers_args}
          cmake --build build --target install
      - name: Google Test
        shell: bash
        run: |
          cd googletest
          cmake \\
            -B build \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
            -GNinja \\
            -DCMAKE_INSTALL_PREFIX=../_install \\
            {compilers_args}
          cmake --build build --target install
      - name: zlib
        shell: bash
        run: |
          cd zlib
          cmake \\
            -B build \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
            -GNinja \\
            -DCMAKE_INSTALL_PREFIX=../_install \\
            {compilers_args}
          cmake --build build --target install

      # sin-cpp
      - name: CMake
        shell: bash
        run: |
          cd sin-cpp
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake -B build -DCMAKE_BUILD_TYPE=Release \\
                                                {build_system} \\
                                                -DCMAKE_INSTALL_PREFIX=../_install \\
                                                -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
                                                {compilers_args}
      - name: Build
        shell: bash
        run: |
          cd sin-cpp/build
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --build .
      - name: Test
        shell: bash
        run: |
          cd sin-cpp/build
          VERBOSE=1 PATH="../_install/bin:$PATH" ctest --output-on-failure
"""

step_windows = """
      # Dependencies
      - name: Google Benchmark
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-google-benchmark
      - name: Google Test
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-google-test
      - name: zlib
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-zlib

      # sin-cpp
      - name: CMake
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --cmake
      - name: Build
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build
      - name: Test
        run: |
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --test
"""

if __name__ == "__main__":

    script_dirname = os.path.dirname(os.path.realpath(__file__))
    output_path = os.path.join(
        script_dirname, "..", ".github", "workflows", "ci_generated.yml"
    )

    with open(output_path, "w") as f:
        print("Writing file", output_path)
        f.write(header)

        for label, configs in config.items():

            os = label[: label.rfind("-")]

            f.write(f"\n  # {os}\n")

            for build_system in configs["build_system"]:
                for compilers_args in configs["compilers_args"]:

                    # Header
                    f.write(
                        runner.format(
                            runner=f"{os}-{build_system['name']}-{compilers_args['name']}".lower(),
                            label=label,
                        )
                    )

                    # Clean
                    if os.startswith("windows"):
                        f.write(step_clean_windows)
                    else:
                        f.write(step_clean_unix)

                    # Clone
                    f.write(step_clone)

                    # Dependencies & sin-cpp
                    if os.startswith("windows"):
                        f.write(
                            step_windows.format(vcvars_path=get_vcvars_paths(label))
                        )
                    else:
                        f.write(
                            step_unix.format(
                                os=os,
                                build_system=build_system["args"],
                                compilers_args=compilers_args["args"],
                            )
                        )

    sys.exit(0)
