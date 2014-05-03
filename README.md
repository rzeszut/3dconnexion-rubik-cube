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

TBD -- probably similar to linux

### Windows

Install MinGW (with g++ and pthreads).
Install CMake with adding it to PATH.

Problems
--------

1. CMake errors:

    make clean
    ./build_debug

