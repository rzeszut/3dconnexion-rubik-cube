#ifndef PROGRAM_HPP_HVHJYI72
#define PROGRAM_HPP_HVHJYI72

#include <stack>
#include <memory>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "util/preconditions.hpp"
#include "gl/data.hpp"
#include "loader.hpp"

#define WITH_PROGRAM(prog) if (bool __p = false) {} else\
                                          for((prog).begin();\
                                              !__p;\
                                              (prog).end(),\
                                              __p = true)

namespace gl {
namespace shader {

class Program {
    GLuint programID;
    std::stack<GLuint> attributes;

public:
    Program(GLuint id);
    ~Program();

    GLuint getID() const;

    void begin();
    void end();

    void setUniform(const char *name, const glm::mat4 &mat);
    void setUniform(const char *name, const glm::vec3 &vec);
    void setUniform(const char *name, int i);

    void setTexture2D(const char *name, int unit, GLuint textureID);

    void setAttribute(const char *name, const BufferInfo &attribute);

    void drawElements(GLenum mode, const ElementsInfo &elements);
    void drawArrays(GLenum mode, GLsizei count);

    static std::unique_ptr<Program> fromFiles(const char *vertexShaderFile, const char* fragmentShaderFile);
};

// ------------------ //
// inline definitions //
// ------------------ //

inline Program::Program(GLuint id) : programID(id) {}

inline Program::~Program() {
    glDeleteProgram(programID);
}

inline GLuint Program::getID() const {
    return programID;
}

inline void Program::begin() {
    glUseProgram(programID);
}

inline void Program::end() {
    while (!attributes.empty()) {
        auto attr = attributes.top();
        glDisableVertexAttribArray(attr);
        attributes.pop();
    }
    glUseProgram(NULL_PROGRAM);
}

inline void Program::setUniform(const char *name, const glm::mat4 &mat) {
    auto uniformLocation = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
}

inline void Program::setUniform(const char *name, const glm::vec3 &vec) {
    auto uniformLocation = glGetUniformLocation(programID, name);
    glUniform3fv(uniformLocation, 1, &vec[0]);
}

inline void Program::setUniform(const char *name, int i) {
    auto uniformLocation = glGetUniformLocation(programID, name);
    glUniform1i(uniformLocation, i);
}

inline void Program::setTexture2D(const char *name, int unit, GLuint textureID) {
    CHECK(unit >= 0 && unit < 8, "Unit must be between 0 and 8");

    auto uniformLocation = glGetUniformLocation(programID, name);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(uniformLocation, unit);
}

inline void Program::setAttribute(const char *name, const BufferInfo &attribute) {
    auto attributeLocation = glGetAttribLocation(programID, name);
    attributes.push(attributeLocation);

    glEnableVertexAttribArray(attributeLocation);
    glBindBuffer(GL_ARRAY_BUFFER, attribute.bufferID);
    glVertexAttribPointer(attributeLocation, attribute.size, attribute.type, GL_FALSE, 0, 0);
}

inline void Program::drawElements(GLenum mode, const ElementsInfo &elements) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements.bufferID);
    glDrawElements(mode, elements.count, elements.type, 0);
}

inline void Program::drawArrays(GLenum mode, GLsizei count) {
    glDrawArrays(mode, 0, count);
}

inline std::unique_ptr<Program> Program::fromFiles(const char *vertexShaderFile,
                                                   const char* fragmentShaderFile) {
    GLuint programID = loadFromFiles(vertexShaderFile, fragmentShaderFile);
    if (programID == NULL_PROGRAM) {
        return nullptr;
    }
    return std::unique_ptr<gl::shader::Program>(new gl::shader::Program(programID));
}

}
}

#endif /* end of include guard: PROGRAM_HPP_HVHJYI72 */

