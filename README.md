3dconnexion-rubik-cube
=====================

Setup
-----

Install Git, if you haven't already.
Clone repository and initialize submodules:

    git clone git@github.com:rzeszut/3dconnexion-rubik-cube.git
    cd 3dconnexion-rubik-cube
    git submodule init
    git submodule update

Compiling and running
-------------

### Linux

Install CMake and g++:

    sudo apt-get install cmake g++ pkg-config

Install dependencies for GLFW:

    sudo apt-get install xorg-dev libglu1-mesa-dev

Compile debug version:

    ./build_debug

Run the program:

    ./run

### Mac

Install CMake and pkg-config:

    brew install cmake
    brew install pkg-config

Install g++:

    brew tap homebrew/versions
    brew install gcc49 --enable-cxx

### Windows

Install [MinGW](http://sourceforge.net/projects/mingwbuilds/) and add it into PATH.
Install [CMake](http://www.cmake.org/cmake/resources/software.html) with adding it to PATH.

Compile debug version:

    ./build_debug.bat

Run the program:

    ./run.bat

Problems
--------

1. CMake errors:

        make clean
        ./build_debug

