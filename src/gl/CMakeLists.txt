add_library(gl-lib STATIC
    error.cpp

    window/abstract_window.cpp
    window/window.cpp

    camera/abstract_camera.cpp
    camera/fps_camera.cpp
    camera/centered_camera.cpp

    texture/texture.cpp
    texture/loader.cpp

    shader/shader.cpp
    shader/program.cpp
    shader/loader.cpp

    mesh/mesh.cpp
    mesh/index.cpp
    mesh/obj_loader.cpp
    )
