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

from typing import Any
import os
import sys

config: dict[str, Any] = {}

# debian + raspios + manjaro
for system in [
    # debian12
    "debian12-amd64",
    "debian12-i386",
    "debian12-armhf",
    "debian12-arm64",
    "debian12-ppc64el",
    # debian13
    "debian13-amd64",
    "debian13-i386",
    "debian13-armhf",
    "debian13-arm64",
    "debian13-ppc64el",
    # raspios12
    "raspios12-armhf",
    "raspios12-arm64",
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
                ]
            },
        }
    )

# macOS
for version in ["14", "15"]:
    system = "macos" + version + "-amd64"
    config.update(
        {
            system: {
                "compilers_args": [
                    {
                        "name": "apple-clang",
                        "args": "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang",
                    },
                    {
                        "name": "clang",
                        "args": '-DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ LDFLAGS="-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -lunwind" CPPFLAGS=-I/usr/local/opt/llvm/include',
                    },
                    {
                        "name": "gcc",
                        "args": f"-DCMAKE_CXX_COMPILER=/usr/local/opt/gcc/bin/g++-14 -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX{version}.sdk",
                    },
                ]
            },
        }
    )
for version in ["14", "15"]:
    system = "macos" + version + "-arm64"
    config.update(
        {
            system: {
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
                        "name": "gcc",
                        "args": f"-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/gcc/bin/g++-14 -DCMAKE_C_COMPILER=/opt/homebrew/opt/gcc/bin/gcc-14 -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX{version}.sdk",
                    },
                ]
            },
        }
    )

# Windows
for system in ["windows10-amd64", "windows10-i386", "windows11-amd64"]:
    config.update(
        {
            system: {
                "compilers_args": [
                    {
                        "name": "msvc",
                        "args": "",
                    },
                ]
            },
        },
    )


def get_working_dir(system: str):
    if system.startswith("windows"):
        return "$env:USERPROFILE\\_ci_sin-cpp"
    else:
        return "$HOME/_ci_sin-cpp"


def get_vcvars_paths(system: str):
    if system == "windows10-i386":
        return "C:\\Program Files\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat"
    else:
        return "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"


header = """# THIS FILE IS GENERATED!

name: ci

on:
  pull_request:

jobs:
"""

runner = """
  {job}:
    continue-on-error: true
    timeout-minutes: 600
    runs-on: [ self-hosted, {system} ]
    name: {job}
    steps:
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

step_clean_and_move_repo_unix = """
      # Clean
      - name: Clean
        run: |
          rm -rf "{working_dir}"
          mkdir -p "{working_dir}"
      # Move Repositories
      - name: Move Repositories
        run: mv * "{working_dir}"
"""

step_clean_and_move_repo_windows = """
      # Clean
      - name: Clean
        run: |
          if (Test-Path "{working_dir}") {{
            Remove-Item -Path "{working_dir}" -Recurse -Force
          }}
          mkdir "{working_dir}"
      # Move Repositories
      - name: Move Repositories
        run: |
          dir
          Move-Item -Path * -Destination "{working_dir}"
"""

step_unix = """
      # Dependencies
      #- name: Google Benchmark
      #  shell: bash
      #  run: |
      #    cd "{working_dir}/benchmark"
      #    cmake \\
      #      -B build \\
      #      -DBENCHMARK_ENABLE_TESTING=OFF \\
      #      -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \\
      #      -DCMAKE_BUILD_TYPE=Release \\
      #      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
      #      -GNinja \\
      #      -DCMAKE_INSTALL_PREFIX=../_install \\
      #      {compilers_args}
      #    cmake --build build --target install
      - name: Google Test
        shell: bash
        run: |
          cd "{working_dir}/googletest"
          cmake \\
            -B build \\
            -DBUILD_GMOCK=OFF \\
            -Dgtest_build_tests=OFF -Dgtest_build_samples=OFF \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
            -GNinja \\
            -DCMAKE_INSTALL_PREFIX=../_install \\
            {compilers_args}
          cmake --build build --target install
      - name: zlib
        shell: bash
        run: |
          cd "{working_dir}/zlib"
          cmake \\
            -B build \\
            -DZLIB_BUILD_TESTING=OFF \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_C_COMPILER_LAUNCHER=ccache \\
            -GNinja \\
            -DCMAKE_INSTALL_PREFIX=../_install \\
            {compilers_args}
          cmake --build build --target install

      # sin-cpp
      - name: CMake
        shell: bash
        run: |
          cd "{working_dir}/sin-cpp"
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake -B build -DCMAKE_BUILD_TYPE=Release \\
          -G"`python3 -c "import random; print(random.choice(['Ninja', 'Unix Makefiles']))"`" \\
          -DCMAKE_INSTALL_PREFIX=../_install \\
          -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \\
          {compilers_args}
      - name: Build
        shell: bash
        run: |
          cd "{working_dir}/sin-cpp/build"
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --version
          VERBOSE=1 PATH="../_install/bin:$PATH" cmake --build .
      - name: Tests
        shell: bash
        run: |
          cd "{working_dir}/sin-cpp/build"
          VERBOSE=1 PATH="../_install/bin:$PATH" ctest --output-on-failure
"""

step_windows = """
      # Dependencies
      - name: Google Benchmark
        run: |
          cd "{working_dir}"
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-google-benchmark
      - name: Google Test
        run: |
          cd "{working_dir}"
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-google-test
      - name: zlib
        run: |
          cd "{working_dir}"
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build-zlib

      # sin-cpp
      - name: CMake
        run: |
          cd "{working_dir}"
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --cmake
      - name: Build
        run: |
          cd "{working_dir}"
          python "sin-cpp\\ci\\windows.py" --vcvars-path "{vcvars_path}" --build
      - name: Test
        run: |
          cd "{working_dir}"
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

        for system, configs in config.items():

            f.write(f"\n  # {system}\n")

            for compilers_args in configs["compilers_args"]:

                # Header
                f.write(
                    runner.format(
                        job=f"{system}-{compilers_args['name']}".lower(),
                        system=system,
                    )
                )

                # Clone
                f.write(step_clone.format(working_dir=get_working_dir(system)))

                # Clean
                if system.startswith("windows"):
                    f.write(
                        step_clean_and_move_repo_windows.format(
                            working_dir=get_working_dir(system)
                        )
                    )
                else:
                    f.write(
                        step_clean_and_move_repo_unix.format(
                            working_dir=get_working_dir(system)
                        )
                    )

                # Dependencies & sin-cpp
                if system.startswith("windows"):
                    f.write(
                        step_windows.format(
                            vcvars_path=get_vcvars_paths(system),
                            working_dir=get_working_dir(system),
                        )
                    )
                else:
                    f.write(
                        step_unix.format(
                            compilers_args=compilers_args["args"],
                            working_dir=get_working_dir(system),
                        )
                    )

    sys.exit(0)
