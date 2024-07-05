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
    "debian12-amd64-sse42",
    "debian13-amd64-sse42",
    "raspios12-armhf-neon",
    # "raspios12-arm64-asimd",
    # "manjaro-aarch64-asimd",
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
for arch in ["arm64-asimd"]:
    config.update(
        {
            "macOS"
            + "-"
            + arch: {
                "compilers_args": [
                    {
                        "name": "clang",
                        "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang",
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
for version in ["10", "11"]:
    config.update(
        {
            "windows"
            + version
            + "-amd64-sse42": {
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
    runs-on: [ self-hosted, {os} ]
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
      - name: Clone Google Test
        uses: actions/checkout@v3
        with:
          repository: "google/googletest"
          path: 'googletest'
      - name: Clone Google Benchmark
        uses: actions/checkout@v3
        with:
          repository: "google/benchmark"
          path: 'benchmark'
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
      - name: Google Test
        shell: bash
        run: |
          cd googletest
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake -B build -DCMAKE_BUILD_TYPE=Release \\
                                                -GNinja \\
                                                -DCMAKE_INSTALL_PREFIX=../_install \\
                                                -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
                                                {compilers_args}
          cmake --build build --target install
      - name: Google Benchmark
        shell: bash
        run: |
          cd benchmark
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake -B build -DCMAKE_BUILD_TYPE=Release \\
                                                -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \\
                                                -GNinja \\
                                                -DCMAKE_INSTALL_PREFIX=../_install \\
                                                -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
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
      # sin-cpp
      - name: Compile & Test
        run: |
          cd sin-cpp
          cmd.exe /c "ci\\windows.bat" "{os}"
"""

if __name__ == "__main__":

    script_dirname = os.path.dirname(os.path.realpath(__file__))
    output_path = os.path.join(
        script_dirname, "..", ".github", "workflows", "ci_generated.yml"
    )

    with open(output_path, "w") as f:
        print("Writing file", output_path)
        f.write(header)

        for os, configs in config.items():

            f.write(f"\n  # {os}\n")

            for build_system in configs["build_system"]:
                for compilers_args in configs["compilers_args"]:

                    # Header
                    f.write(
                        runner.format(
                            runner=f"{os}-{build_system['name']}-{compilers_args['name']}".lower(),
                            os=os,
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
                        f.write(step_windows.format(os=os))
                    else:
                        f.write(
                            step_unix.format(
                                os=os,
                                build_system=build_system["args"],
                                compilers_args=compilers_args["args"],
                            )
                        )

    sys.exit(0)
