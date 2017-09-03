#!/bin/bash
cd "$(dirname "$0")"
mkdir -p build-win
rm -rf build-win/*
cd build-win
cmake -DCMAKE_TOOLCHAIN_FILE=../src/basic-mingw-w64-64.cmake -DCMAKE_BUILD_TYPE=Debug ../src
make -j4
wine exepeek.exe
cd ..
