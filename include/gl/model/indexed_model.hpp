#ifndef INDEXED_MODEL_HPP_R5SU6CHL
#define INDEXED_MODEL_HPP_R5SU6CHL

#include <memory>

#include "model.hpp"

namespace gl {
namespace model {

class IndexedModel : public Model {
private:
    std::vector<unsigned> indices;
    ElementsInfo elementsBuffer;

public:
    IndexedModel(const std::vector<glm::vec3> &vertices,
                 const std::vector<glm::vec3> &normals,
                 const std::vector<glm::vec2> &uvs,
                 const std::vector<unsigned> &indices);

    void init();
    void cleanup();

    const std::vector<unsigned> &getIndices() const;
    GLuint getElementBufferID() const;
    const ElementsInfo &getElementBufferInfo() const;

    static std::unique_ptr<IndexedModel> fromModel(std::unique_ptr<Model> model);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline IndexedModel::IndexedModel(const std::vector<glm::vec3> &v,
                                  const std::vector<glm::vec3> &n,
                                  const std::vector<glm::vec2> &u,
                                  const std::vector<unsigned> &i) :
    Model{v, n, u}, indices{i}
{}

inline const std::vector<unsigned> &IndexedModel::getIndices() const {
    return indices;
}

inline GLuint IndexedModel::getElementBufferID() const {
    return elementsBuffer.bufferID;
}

inline const ElementsInfo &IndexedModel::getElementBufferInfo() const {
    return elementsBuffer;
}

}
}

#endif /* end of include guard: INDEXED_MODEL_HPP_R5SU6CHL */

