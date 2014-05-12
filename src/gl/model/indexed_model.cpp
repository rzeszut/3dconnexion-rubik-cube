#include <cstring>
#include <map>

#include "gl/model/indexed_model.hpp"

using namespace std;
using namespace glm;

namespace gl {
namespace model {

void IndexedModel::init() {
    Model::init();

    // element buffer creation
    glGenBuffers(1, &elementsBuffer.bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer.bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), &indices[0], GL_STATIC_DRAW);
    elementsBuffer.count = indices.size();
    elementsBuffer.type = GL_UNSIGNED_INT;
}

void IndexedModel::cleanup() {
    Model::cleanup();

    glDeleteBuffers(1, &elementsBuffer.bufferID);
}

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;

    bool operator< (const Vertex &other) const {
        return memcmp(this, &other, sizeof(Vertex)) > 0;
    }
};

unique_ptr<IndexedModel> IndexedModel::fromModel(unique_ptr<Model> model) {
    unsigned index = 0;
    map<Vertex, unsigned> indexMap;

    vector<unsigned> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> uvs;

    const int size = model->getVertices().size();
    for (int i = 0; i < size; ++i) {
        Vertex v = {model->getVertices()[i], model->getNormals()[i], model->getUVs()[i]};

        auto it = indexMap.find(v);
        if (it == indexMap.end()) {
            // new vertex
            indexMap[v] = index;
            indices.push_back(index);
            vertices.push_back(v.position);
            normals.push_back(v.normal);
            uvs.push_back(v.uv);
            ++index;
        } else {
            // already indexed
            indices.push_back(it->second);
        }
    }

    auto *indexed = new IndexedModel{vertices, normals, uvs, indices};
    return unique_ptr<IndexedModel>(indexed);
}

}
}

