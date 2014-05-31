#ifndef MESH_HPP_9EBXPSOV
#define MESH_HPP_9EBXPSOV

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "gl/buffer.hpp"

namespace gl {
namespace mesh {

class Mesh {
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned> elements;

    BufferInfo vertexBuffer;
    BufferInfo normalBuffer;
    BufferInfo uvBuffer;
    ElementBufferInfo elementBuffer;

    bool initialized = false;

    Mesh() = default;

public:
    virtual ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh &operator =(const Mesh &) = delete;

    void init();

    const BufferInfo &getVertexBufferInfo() const;
    const BufferInfo &getNormalBufferInfo() const;
    const BufferInfo &getUVBufferInfo() const;
    const ElementBufferInfo &getElementBufferInfo() const;

    static std::unique_ptr<Mesh> fromOBJFile(const char *filename);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline const BufferInfo &Mesh::getVertexBufferInfo() const {
    return vertexBuffer;
}

inline const BufferInfo &Mesh::getNormalBufferInfo() const {
    return normalBuffer;
}

inline const BufferInfo &Mesh::getUVBufferInfo() const {
    return uvBuffer;
}

inline const ElementBufferInfo &Mesh::getElementBufferInfo() const {
    return elementBuffer;
}

}
}

#endif /* end of include guard: MESH_HPP_9EBXPSOV */

