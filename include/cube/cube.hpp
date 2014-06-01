#ifndef CUBE_HPP_LEFAYCZB
#define CUBE_HPP_LEFAYCZB

#include <array>
#include <tuple>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "util/optional.hpp"
#include "gl/mesh/mesh.hpp"
#include "gl/shader/program.hpp"
#include "gl/texture/texture.hpp"
#include "gl/picking/ray.hpp"

namespace cube {

constexpr int NUMBER_OF_CUBES = 3 * 3 * 3;

typedef std::tuple<int, int, int> Coords;

enum class Axis {
    X, Y, Z
};

optional::Optional<std::pair<Axis, int>> findCommonAxis(const Coords &c1,
                                                        const Coords &c2,
                                                        const Coords &c3);

class Cube {
private:
    std::unique_ptr<gl::mesh::Mesh> mesh;
    std::unique_ptr<gl::shader::Program> program;
    std::unique_ptr<gl::texture::Texture> texture;

    std::array<glm::mat4, NUMBER_OF_CUBES> modelMatrices;
    std::array<glm::quat, NUMBER_OF_CUBES> rotations;

private:
    int getID(int i, int j, int k) const;
    Coords getCoords(int id) const;

    void rotateX(int x, float angle);
    void rotateY(int y, float angle);
    void rotateZ(int z, float angle);

public:
    void init();

    void render(const glm::mat4 &view, const glm::mat4 &projection);

    optional::Optional<Coords> testRayIntersection(const gl::picking::Ray &ray);

    void rotate(Axis axis, int coord, float angle);
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

