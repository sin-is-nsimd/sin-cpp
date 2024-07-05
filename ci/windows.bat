call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86

set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\VC\SecurityIssueAnalysis\python;%PATH%
echo "PATH = %PATH%"

cd ".."

cd "googletest"
mkdir "build"
cd "build"
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ninja install
cd ..
cd ..

cd "benchmark"
mkdir "build"
cd "build"
cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ninja install
cd ..
cd ..

cd "sin-cpp"
mkdir "build_ci_%~1"
cd "build_ci_%~1"
cmake .. -DSINCPP_LIB_TYPE=STATIC -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_INSTALL_PREFIX="../../_install"
ninja
ctest  --output-on-failure
