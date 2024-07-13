call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86

set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\VC\SecurityIssueAnalysis\python;%PATH%
echo "PATH = %PATH%"

cd ".."

cd "benchmark"
mkdir "build"
cd "build"
cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ninja install
cd ..
cd ..

cd "googletest"
mkdir "build"
cd "build"
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ninja install
cd ..
cd ..

cd "zlib"
mkdir "build"
cd "build"
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ninja install
cd ..
cd ..

cd "sin-cpp"
mkdir "build"
cd "build"
cmake .. -DSINCPP_LIB_TYPE=STATIC -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ctest --output-on-failure
