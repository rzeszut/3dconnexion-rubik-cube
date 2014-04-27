#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "gl/shader.hpp"
#include "logging/logging.hpp"

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

static const GLuint NULL_SHADER = 0;

static GLuint createShader(GLenum shaderType, const char *code) {
    // create shader
    GLuint shader = glCreateShader(shaderType);

    // compile shader
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

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
        LOG(ERROR) << &errorMessages[0];

        // delete shader
        glDeleteShader(shader);
        shader = NULL_SHADER;
    }

    return shader;
}

GLuint loadFromSource(const char *vertexSource, const char *fragmentSource) {
    // create the shaders
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    // check for errors in shader compilation
    if (vertexShader == NULL_SHADER) {
        return NULL_PROGRAM;
    }
    if (fragmentShader == NULL_SHADER) {
        glDeleteShader(vertexShader);
        return NULL_PROGRAM;
    }

    // link the program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // mark shaders for deletion
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // detach them from the program
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

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

        // delete program
        glDeleteProgram(program);
        program = NULL_PROGRAM;
    }

    return program;
}

GLuint loadFromFiles(const char *vertexFilePath, const char *fragmentFilePath) {
    // read the vertex shader code from file
    auto vertexShaderCode = readFile(vertexFilePath);
    if (vertexShaderCode.empty()) {
        return NULL_PROGRAM;
    }

    // read the fragment shader code from file
    auto fragmentShaderCode = readFile(fragmentFilePath);
    if (fragmentShaderCode.empty()) {
        return NULL_PROGRAM;
    }

    return loadFromSource(vertexShaderCode.data(), fragmentShaderCode.data());
}

}
}

