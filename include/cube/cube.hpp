#ifndef CUBE_HPP_LEFAYCZB
#define CUBE_HPP_LEFAYCZB

#include <array>

#include "glm/glm.hpp"

#include "gl/mesh/mesh.hpp"
#include "gl/shader/program.hpp"
#include "gl/texture/texture.hpp"

namespace cube {

class Cube {
private:
    std::unique_ptr<gl::mesh::Mesh> mesh;
    std::unique_ptr<gl::shader::Program> program;
    std::unique_ptr<gl::texture::Texture> texture;

    std::array<glm::mat4, 3 * 3 * 3> modelMatrices;

    glm::mat4 &getModelMatrix(int i, int j, int k);

public:
    void init();

    void render(const glm::mat4 &view, const glm::mat4 &projection);
};

}

#endif /* end of include guard: CUBE_HPP_LEFAYCZB */

