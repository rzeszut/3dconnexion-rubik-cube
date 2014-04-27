cmake_minimum_required(VERSION 2.8)
project(3dconnexion-rubik-cube)

# Set this to on (-Dtest=ON) to compile tests as well
option(test "Build all tests." OFF)

# Basic variables
set(MAINFOLDER ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
set(LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")
set(INCLUDES "${MAINFOLDER}/include")
set(SOURCES "${MAINFOLDER}/src")
set(THIRDPARTY "${MAINFOLDER}/thirdparty")
set(TESTDIR "${MAINFOLDER}/test")

# Add modules to search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Compiler settings
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-comment -g -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wno-comment -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Additional compiler flags
# http://public.kitware.com/pipermail/cmake/2013-January/053117.html
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${MAINFOLDER}/,,$(abspath $<))\"'")

# add glfw3
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples.")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests.")
add_subdirectory(thirdparty/glfw)
include_directories(thirdparty/glfw/include)

# add glew
add_subdirectory(thirdparty/glew-cmake)
include_directories(thirdparty/glew-cmake/include)

# add glm
add_definitions(-DGLM_FORCE_RADIANS)
include_directories(thirdparty/glm)

include_directories(${INCLUDES})
add_subdirectory(${SOURCES})

if (test)
    add_subdirectory(${THIRDPARTY}/gmock)
    enable_testing()

    include_directories(${THIRDPARTY}/gmock/include)
    include_directories(${THIRDPARTY}/gmock/gtest/include)
    add_subdirectory(${TESTDIR})
endif()
