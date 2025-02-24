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

import argparse
import os
import sys

sys.path.append(
    os.path.join(
        os.path.dirname(os.path.realpath(__file__)), "..", "..", "sin-python", "src"
    )
)
import sin.fs
import sin.sh


def load_vcvars(vcvars_path):
    # https://stackoverflow.com/questions/14697629/running-a-bat-file-though-python-in-current-process
    output = os.popen('"{}" && set'.format(vcvars_path)).read()
    for line in output.splitlines():
        pair = line.split("=", 1)
        if len(pair) >= 2:
            os.environ[pair[0]] = pair[1]


def run_cmake(cmake_args=""):
    sin.sh.run_cmd(
        'cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../_install" '
        + cmake_args
    )


def build_dep(build_dir, cmake_args=""):
    os.mkdir(build_dir)
    with sin.fs.pushd(build_dir):
        run_cmake(cmake_args)
        sin.sh.run_cmd("ninja -v")
        sin.sh.run_cmd("ninja install")


if __name__ == "__main__":

    # Args

    parser = argparse.ArgumentParser(description="Compile and test sin-cpp")

    parser.add_argument("--vcvars-path", default=None)

    parser.add_argument(
        "--build-google-benchmark", help="Build Google Benchmark", action="store_true"
    )
    parser.add_argument(
        "--build-google-test", help="Build Google Test", action="store_true"
    )
    parser.add_argument("--build-zlib", help="Build zlib", action="store_true")

    parser.add_argument("--cmake", help="CMake sin-cpp", action="store_true")
    parser.add_argument("--build", help="Build sin-cpp", action="store_true")
    parser.add_argument("--test", help="Test sin-cpp", action="store_true")

    args = parser.parse_args()

    # vcvars*.bat

    print("LOAD", args.vcvars_path)
    load_vcvars(args.vcvars_path)

    # Steps

    if args.build_google_benchmark:
        print("BUILD GOOGLE BENCHMARK")
        build_dep(
            "benchmark/build",
            "-DBENCHMARK_ENABLE_GTEST_TESTS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded",
        )

    if args.build_google_test:
        print("BUILD GOOGLE TEST")
        build_dep("googletest/build", "-DBUILD_GMOCK=OFF -Dgtest_build_tests=OFF -Dgtest_build_samples=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded")

    if args.build_zlib:
        print("BUILD ZLIB")
        build_dep("zlib/build", "-DZLIB_BUILD_SHARED=OFF -DZLIB_BUILD_TESTING=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded")

    if args.cmake:
        print("CMAKE SIN-CPP")
        os.mkdir("sin-cpp/build")
        with sin.fs.pushd("sin-cpp/build"):
            run_cmake('-DSINCPP_LIB_TYPE=STATIC -DCMAKE_CXX_FLAGS_RELEASE="/MT"')

    if args.build:
        print("BUILD SIN-CPP")
        with sin.fs.pushd("sin-cpp/build"):
            sin.sh.run_cmd("ninja -v")

    if args.test:
        print("TEST SIN-CPP")
        with sin.fs.pushd("sin-cpp/build"):
            sin.sh.run_cmd("ctest --output-on-failure")

    # Exit

    sys.exit(0)
