#include "gl/model/model.hpp"

namespace gl {
namespace model {

void Model::init() {
    // vertex buffer creation
    glGenBuffers(1, &vertexBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    vertexBuffer.size = 3;
    vertexBuffer.type = GL_FLOAT;

    // normal buffer creation
    glGenBuffers(1, &normalBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    normalBuffer.size = 3;
    normalBuffer.type = GL_FLOAT;

    // uv buffer creation
    glGenBuffers(1, &uvBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
    uvBuffer.size = 2;
    uvBuffer.type = GL_FLOAT;
}

void Model::cleanup() {
    glDeleteBuffers(1, &vertexBuffer.bufferID);
    glDeleteBuffers(1, &uvBuffer.bufferID);
    glDeleteBuffers(1, &normalBuffer.bufferID);
}

}
}

