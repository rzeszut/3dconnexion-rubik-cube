#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "logging/logging.hpp"
#include "gl/error.hpp"
#include "gl/shader/shader.hpp"
#include "gl/shader/loader.hpp"

using namespace std;

namespace gl {
namespace shader {

static GLenum getGLShaderType(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;

    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER;

    default:
        return 0;
    }
}

shared_ptr<Shader> Shader::fromFile(const char *filename, ShaderType type) {
    GLuint shader = loadShaderFromFile(filename, getGLShaderType(type));
    if (shader == NULL_SHADER) {
        return nullptr;
    } else {
        // can't use make_shader() function, because it calls Shader destructor,
        // which cleans up GL data
        return shared_ptr<Shader>(new Shader(shader, type));
    }
}

shared_ptr<Shader> Shader::fromSource(const char *source, ShaderType type) {
    GLuint shader = loadShaderFromSource(source, getGLShaderType(type));
    if (shader == NULL_SHADER) {
        return nullptr;
    } else {
        return shared_ptr<Shader>(new Shader(shader, type));
    }
}

}
}

