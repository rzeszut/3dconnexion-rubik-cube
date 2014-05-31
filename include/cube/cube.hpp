#ifndef CUBE_HPP_LEFAYCZB
#define CUBE_HPP_LEFAYCZB

#include <array>
#include <tuple>

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

public:
    void init();

    void render(const glm::mat4 &view, const glm::mat4 &projection);

    glm::mat4 &getModelMatrix(int i, int j, int k);
    const glm::mat4 &getModelMatrix(int i, int j, int k) const;

};

typedef std::tuple<int, int, int> Coords;

std::ostream & operator<<(std::ostream &stream, const Coords &coords);

// ------------------ //
// inline definitions //
// ------------------ //

inline std::ostream & operator<<(std::ostream &stream, const Coords &coords) {
    stream << "<" << std::get<0>(coords)
        << ", " << std::get<1>(coords)
        << ", " << std::get<2>(coords)
        << ">";
    return stream;
}

}

#endif /* end of include guard: CUBE_HPP_LEFAYCZB */

