#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "logging/logging.hpp"
#include "gl/error.hpp"
#include "gl/shader/loader.hpp"

using namespace std;

namespace gl {
namespace shader {

static vector<char> readFile(const char *filename) {
    LOG(INFO) << "Loading shader file: " << filename;

    ifstream file(filename, ios::in);

    if (file) {
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        vector<char> ret(length + 1, 0);
        file.read(&ret[0], length);
        file.close();

        return ret;
    }

    return {};
}

GLuint loadShaderFromFile(const char *filename, GLenum type) {
    // read the shader code from file
    auto source = readFile(filename);
    if (source.empty()) {
        return NULL_SHADER;
    }

    return loadShaderFromSource(source.data(), type);
}

GLuint loadShaderFromSource(const char *source, GLenum type) {
    // create shader
    GLuint shader = glCreateShader(type);

    // compile shader
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    logGLError();

    // check vertex shader
    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        // get error log length
        int infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        // get log
        vector<char> errorMessages(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessages[0]);
        LOG(ERROR) << std::endl << &errorMessages[0];

        // delete shader
        glDeleteShader(shader);

        return NULL_SHADER;
    }

    return shader;
}

GLuint compileProgram(const std::vector<GLuint> &shaders) {
    GLuint program = glCreateProgram();

    // attach shaders to the program
    for (auto &it : shaders) {
        if (it == NULL_SHADER) {
            glDeleteProgram(program);
            return NULL_PROGRAM;
        }
        glAttachShader(program, it);
        logGLError();
    }

    // link the program
    glLinkProgram(program);
    logGLError();

    // detach shaders from the program
    for (auto &it : shaders) {
        glDetachShader(program, it);
    }

    // check the program
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE) {
        // get log length
        int infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        // get log
        vector<char> programErrorMessages(max(infoLogLength, int(1)));
        glGetProgramInfoLog(program, infoLogLength, NULL, &programErrorMessages[0]);
        LOG(ERROR) << &programErrorMessages[0];

        glDeleteProgram(program);
        return NULL_PROGRAM;
    }

    return program;
}

}
}

