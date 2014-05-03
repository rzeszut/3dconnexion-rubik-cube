@echo off
mkdir build
cd build
cmake .. -Dtest=OFF -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
mingw32-make.exe