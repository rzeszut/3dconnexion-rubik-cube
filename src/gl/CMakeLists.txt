add_library(gl-lib STATIC
    texture.cpp
    window/abstract_window.cpp
    window/window.cpp
    camera/abstract_camera.cpp
    camera/fps_camera.cpp
    camera/centered_camera.cpp
    shader/loader.cpp
    model/obj_model_loader.cpp
    model/model.cpp
    model/indexed_model.cpp
    )
