#include "gl/mesh/mesh.hpp"
#include "gl/mesh/obj_loader.hpp"
#include "gl/mesh/index.hpp"

using namespace std;
using namespace glm;

namespace gl {
namespace mesh {

void Mesh::init() {
    // vertex buffer creation
    glGenBuffers(1, &vertexBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    vertexBuffer.size = 3;

    // normal buffer creation
    glGenBuffers(1, &normalBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    normalBuffer.size = 3;

    // uv buffer creation
    glGenBuffers(1, &uvBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
    uvBuffer.size = 2;

    // element buffer creation
    glGenBuffers(1, &elementBuffer.bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * elements.size(), &elements[0], GL_STATIC_DRAW);
    elementBuffer.count = elements.size();

    initialized = true;
}

Mesh::~Mesh() {
    if (initialized) {
        glDeleteBuffers(1, &vertexBuffer.bufferID);
        glDeleteBuffers(1, &uvBuffer.bufferID);
        glDeleteBuffers(1, &normalBuffer.bufferID);
        glDeleteBuffers(1, &elementBuffer.bufferID);
    }
}

unique_ptr<Mesh> Mesh::fromOBJFile(const char *filename) {
    vector<vec3> verticesTemp;
    vector<vec3> normalsTemp;
    vector<vec2> uvsTemp;

    if (!loadOBJ(filename, verticesTemp, normalsTemp, uvsTemp)) {
        return nullptr;
    }

    auto model = new Mesh;

    indexMesh(verticesTemp, normalsTemp, uvsTemp,
               model->vertices, model->normals, model->uvs, model->elements);

    return unique_ptr<Mesh>(model);
}

}
}

