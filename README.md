3dconnexion-rubik-cube
=====================

Setup
-----

Clone repository and initialize submodules:

    git clone git@github.com:rzeszut/3dconnexion-rubik-cube.git
    git submodule init
    git submodule update

Compiling and running
-------------

Install CMake and g++:

    sudo apt-get install cmake g++ pkg-config

Install dependencies for GLFW:

    sudo apt-get install xorg-dev libglu1-mesa-dev

Compile debug version:

    ./build_debug

Run the program:

    ./bin/cube

Problems
--------

1. CMake errors:

    make clean
    ./build_debug

