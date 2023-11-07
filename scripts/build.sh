#!/bin/bash
cd "$(dirname "$0")"
cd ..

# clang is used as the default compiler
CXX_COMPILER="clang++"
C_COMPILER="clang"
BUILD_TESTS=false
BUILD_ONLY_TEST=false
BUILD_TYPE="Debug"
BUILD_TYPE_NAME="debug"
for var in "$@"
do
    case $var in
        "--clang")
            C_COMPILER="clang"
            CXX_COMPILER="clang++"
            ;;
        "--gcc")
            C_COMPILER="gcc"
            CXX_COMPILER="g++"
            ;;
        "--test")
            BUILD_TESTS=true
            ;;
        "--test-only")
            BUILD_TESTS=true
            BUILD_ONLY_TEST=true
            ;;
        "--debug")
            BUILD_TYPE="Debug"
            BUILD_TYPE_NAME="debug"
            ;;
        "--release")
            BUILD_TYPE="Release"
            BUILD_TYPE_NAME="release"
            ;;
        *);;
    esac
done


if [ ! -d build/linux-$C_COMPILER-$BUILD_TYPE_NAME ]; then
    mkdir build
    mkdir build/linux-$C_COMPILER-$BUILD_TYPE_NAME
    cd build/linux-$C_COMPILER-$BUILD_TYPE_NAME
    cmake -G Ninja -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
fi

cd build/linux-$C_COMPILER-$BUILD_TYPE_NAME/
if [ $BUILD_ONLY_TEST = false ]; then
    cmake --build . -j
fi

if [ $BUILD_TESTS = true ]; then
    cmake --build . -j --target gaea-tests
fi

if [ -e compile_commands.json ]; then
    cp compile_commands.json ../..
fi

