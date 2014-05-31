#ifndef SHADER_HPP_HNFOM1UC
#define SHADER_HPP_HNFOM1UC

#include <memory>
#include <GL/glew.h>

namespace gl {
namespace shader {

enum class ShaderType {
    VERTEX, FRAGMENT
};

class Shader {
    GLuint id;
    ShaderType type;

    Shader(GLuint id, ShaderType type);

public:
    ~Shader();

    Shader() = delete;
    Shader &operator =(const Shader &) = delete;

    GLuint getID() const;
    ShaderType getType() const;

    static std::shared_ptr<Shader> fromFile(const char *filename, ShaderType type);
    static std::shared_ptr<Shader> fromSource(const char *source, ShaderType type);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Shader::Shader(GLuint i, ShaderType t) :
    id(i), type(t)
{}

inline Shader::~Shader() {
    glDeleteShader(id);
}

inline GLuint Shader::getID() const {
    return id;
}

inline ShaderType Shader::getType() const {
    return type;
}

}
}

#endif /* end of include guard: SHADER_HPP_HNFOM1UC */

