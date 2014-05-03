find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(cube
    main.cpp
    main_handler.cpp
    )

target_link_libraries(cube logging)
target_link_libraries(cube gl-lib)
target_link_libraries(cube ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(cube ${OPENGL_LIBRARIES})
target_link_libraries(cube glfw ${glfw_LIBRARIES})
target_link_libraries(cube libglew_shared ${glew_LIBRARIES})

if (WIN32)
    # windows specific
elseif (APPLE)
    # apple specific
elseif (UNIX)
    # must add this for gflw linking -- apparently, dynamic libraries must now be linked
    # linux only -- x11
    target_link_libraries(cube ${X11_LIBRARIES})
    target_link_libraries(cube -lrt -lXinerama -lXrandr -lXxf86vm -lXi -lX11 -lXext)
else ()
    message(FATAL_ERROR "OS not supported.")
endif ()

