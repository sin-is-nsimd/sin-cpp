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

header = """# THIS FILE IS GENERATED!

name: ci

on:
  # push:
  #   branches: [ main ]
  pull_request:

jobs:
"""

runner_header = """
  {runner}:
    continue-on-error: true
    runs-on: [ self-hosted, {os_name}, {arch} ]
    name: {runner}
    steps:
      - name: Clean
        run: rm -rf *
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
      - name: Clone CMake
        uses: actions/checkout@v3
        with:
          repository: Kitware/CMake
          ref: 'v3.29.2'
          path: 'CMake'
"""

step_cmake_build = """
      - name: CMake v3.29.2
        shell: bash
        run: |
          cd CMake
          mkdir build
          cd build
          ../bootstrap --prefix=../../_install -- -DCMAKE_USE_OPENSSL=OFF -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
          make -j `nproc`
          make install
"""

step_unix = """
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
      - name: Build & Test
        shell: bash
        run: |
          cd sin-cpp/build
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --build .
          VERBOSE=1 PATH="../_install/bin:$PATH" ctest --output-on-failure
"""

config = {
    "Linux": {
        "compilers_args": [
            {"name": "gcc", "args": "-DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc"},
            {"name": "clang", "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"},
        ],
        "build_system": [{"name": "ninja", "args": "-GNinja"}, {"name": "make", "args": "-G\"Unix Makefiles\""}],
        "arch": ["X64", "ARM64"],
    },
    "macOS": {
        "compilers_args": [{"name": "clang", "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"}],
        "build_system": [{"name": "ninja", "args": "-GNinja"}, {"name": "make", "args": "-G\"Unix Makefiles\""}],
        "arch": ["ARM64"],
    },
}

if __name__ == "__main__":

    script_dirname = os.path.dirname(os.path.realpath(__file__))
    output_path = os.path.join(
        script_dirname, "..", ".github", "workflows", "ci_generated.yml"
    )

    with open(output_path, "w") as f:
        print("Writing file", output_path)
        f.write(header)

        for os_name, configs in config.items():

            f.write(f"\n  # {os_name}\n")

            for build_system in configs["build_system"]:
                for arch in configs["arch"]:
                    for compilers_args in configs["compilers_args"]:
                        f.write(
                            runner_header.format(
                                runner=f"{os_name}-{arch}-{build_system['name']}-{compilers_args['name']}".lower(),
                                os_name=os_name,
                                arch=arch,
                                compiler_name=compilers_args["name"],
                                compilers_args=compilers_args["args"],
                                build_system=build_system["args"],
                            )
                        )
                        if os_name != "macOS":
                            f.write(step_cmake_build)
                        f.write(
                            step_unix.format(
                                os_name=os_name,
                                arch=arch,
                                compiler_name=compilers_args["name"],
                                compilers_args=compilers_args["args"],
                                build_system=build_system["args"],
                            )
                        )

    sys.exit(0)
