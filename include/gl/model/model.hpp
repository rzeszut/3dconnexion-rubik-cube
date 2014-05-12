#ifndef MODEL_HPP_EU8Z3YGP
#define MODEL_HPP_EU8Z3YGP

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "gl/data.hpp"

namespace gl {
namespace model {

class Model {
protected:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    BufferInfo vertexBuffer;
    BufferInfo normalBuffer;
    BufferInfo uvBuffer;

public:
    Model(const std::vector<glm::vec3> &vertices,
          const std::vector<glm::vec3> &normals,
          const std::vector<glm::vec2> &uvs);
    virtual ~Model() = default;

    virtual void init();
    virtual void cleanup();

    const std::vector<glm::vec3> &getVertices() const;
    const std::vector<glm::vec3> &getNormals() const;
    const std::vector<glm::vec2> &getUVs() const;

    const BufferInfo &getVertexBufferInfo() const;
    const BufferInfo &getNormalBufferInfo() const;
    const BufferInfo &getUVBufferInfo() const;

    GLuint getVertexBufferID() const;
    GLuint getNormalBufferID() const;
    GLuint getUVBufferID() const;
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Model::Model(const std::vector<glm::vec3> &v,
                    const std::vector<glm::vec3> &n,
                    const std::vector<glm::vec2> &u) :
    vertices{v}, normals{n}, uvs{u}
{}

inline const std::vector<glm::vec3> &Model::getVertices() const {
    return vertices;
}

inline const std::vector<glm::vec3> &Model::getNormals() const {
    return normals;
}

inline const std::vector<glm::vec2> &Model::getUVs() const {
    return uvs;
}

inline GLuint Model::getVertexBufferID() const {
    return vertexBuffer.bufferID;
}

inline GLuint Model::getNormalBufferID() const {
    return normalBuffer.bufferID;
}

inline GLuint Model::getUVBufferID() const {
    return uvBuffer.bufferID;
}

inline const BufferInfo &Model::getVertexBufferInfo() const {
    return vertexBuffer;
}

inline const BufferInfo &Model::getNormalBufferInfo() const {
    return normalBuffer;
}

inline const BufferInfo &Model::getUVBufferInfo() const {
    return uvBuffer;
}

}
}

#endif /* end of include guard: MODEL_HPP_EU8Z3YGP */

