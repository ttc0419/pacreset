#!/bin/bash
rm -rf build/
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -B build
cd build
ninja
