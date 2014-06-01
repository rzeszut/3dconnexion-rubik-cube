#ifndef CUBE_HPP_LEFAYCZB
#define CUBE_HPP_LEFAYCZB

#include <array>
#include <tuple>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "gl/mesh/mesh.hpp"
#include "gl/shader/program.hpp"
#include "gl/texture/texture.hpp"
#include "gl/picking/ray.hpp"

namespace cube {

typedef std::tuple<int, int, int> Coords;

class Cube {
private:
    std::unique_ptr<gl::mesh::Mesh> mesh;
    std::unique_ptr<gl::shader::Program> program;
    std::unique_ptr<gl::texture::Texture> texture;

    std::array<glm::mat4, 3 * 3 * 3> modelMatrices;
    std::array<glm::quat, 3 * 3 * 3> rotations;

    int getID(int i, int j, int k) const;
    Coords getCoords(int id) const;

public:
    void init();

    void render(const glm::mat4 &view, const glm::mat4 &projection);

    std::pair<bool, Coords> testRayIntersection(const gl::picking::Ray &ray);

    void rotateX(int x, float angle);
    void rotateY(int y, float angle);
    void rotateZ(int a, float angle);
};

std::ostream & operator<<(std::ostream &stream, const Coords &coords);

// ------------------ //
// inline definitions //
// ------------------ //

inline int Cube::getID(int i, int j, int k) const {
    return 3 * 3 * (i + 1) + 3 * (j + 1) + (k + 1);
}

inline Coords Cube::getCoords(int id) const {
    int x, y, z;
    x = id / 9;
    y = (id - x * 9) / 3;
    z = id % 3;
    return std::make_tuple(x - 1, y - 1, z - 1);
}

inline std::ostream & operator<<(std::ostream &stream, const Coords &coords) {
    stream << "<" << std::get<0>(coords)
        << ", " << std::get<1>(coords)
        << ", " << std::get<2>(coords)
        << ">";
    return stream;
}

}

#endif /* end of include guard: CUBE_HPP_LEFAYCZB */

