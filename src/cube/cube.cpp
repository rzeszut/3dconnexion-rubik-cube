#include <stdexcept>
#include <list>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "logging/logging.hpp"
#include "util/preconditions.hpp"
#include "gl/shader/shader.hpp"
#include "cube/cube.hpp"

using namespace gl::mesh;
using namespace gl::shader;
using namespace gl::texture;
using namespace optional;
using namespace std;

namespace cube {

void Cube::init() {
    // load model
    mesh = Mesh::fromOBJFile("resources/cube.obj");
    if (!mesh) {
        throw std::runtime_error("Error while loading mesh.");
    }
    mesh->init();

    // load shaders
    auto vertexShader = Shader::fromFile("resources/vertex.vert", ShaderType::VERTEX);
    auto fragmentShader = Shader::fromFile("resources/fragment.frag", ShaderType::FRAGMENT);
    program = Program::fromShaders({vertexShader, fragmentShader});
    if (!program) {
        throw std::runtime_error("Error while loading program.");
    }

    // load texture
    texture = Texture::fromDDSFile("resources/uvmap.dds");
    if (!texture) {
        throw std::runtime_error("Error while loading texture.");
    }
    texture->setWrap(TextureWrap::REPEAT, TextureWrap::REPEAT);
    texture->setMinificationFilter(TextureFilter::LINEAR_MIPMAP_LINEAR);
    texture->setMagnificationFilter(TextureFilter::LINEAR);

    // initialize matrices
    for (int i : {-1, 0, 1}) {
        for (int j : {-1, 0, 1}) {
            for (int k : {-1, 0, 1}) {
                auto &model = modelMatrices.at(getID(i, j, k));
                model = glm::translate(model, glm::vec3(2 * i, 2 * j, 2 * k));
            }
        }
    }
}

void Cube::render(const glm::mat4 &view, const glm::mat4 &projection) {
    for (int i = 0; i < NUMBER_OF_CUBES; ++i) {
        const auto &modelMatrix = modelMatrices.at(i);
        const auto &modelRotation = rotations.at(i);
        const auto model = glm::toMat4(modelRotation) * modelMatrix;

        program->use([this, model, view, projection] () {
            glm::mat4 modelViewProjection = projection * view * model;

            program->setUniform("MVP", modelViewProjection);
            program->setTexture2D("textureSampler", 0, texture->getID());

            program->setAttribute("vertexPosition", mesh->getVertexBufferInfo());
            program->setAttribute("vertexUV", mesh->getUVBufferInfo());

            program->drawElements(GL_TRIANGLES, mesh->getElementBufferInfo());
        });
    }
}

Optional<Coords> Cube::testRayIntersection(const gl::picking::Ray &ray) {
    // same bounding box for all cubes
    static gl::picking::AABB aabb(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));

    // a list of all matched pairs: distance, coords of the cube
    std::list<std::pair<float, Coords>> matched;

    for (int i = 0; i < NUMBER_OF_CUBES; ++i) {
        // actual position, rotation included
        const auto &modelMatrix = modelMatrices.at(i);
        const auto &modelRotation = rotations.at(i);
        const auto model = glm::toMat4(modelRotation) * modelMatrix;
        auto result = gl::picking::testRayOBBIntersection(ray, aabb, model);

        if (result) {
            auto coords = getCoords(i);
            matched.push_back(std::make_pair(result.get(), coords));
            LOG(DEBUG) << "Matched cube " << coords << ", distance: " << result.get();
        }
    }

    // sort by distances
    matched.sort([] (std::pair<float, Coords> o1, std::pair<float, Coords> o2) {
        return o1.first < o2.first;
    });

    // the first once (the closes one was clicked)
    if (!matched.empty()) {
        auto picked = matched.front().second;
        LOG(DEBUG) << "Picked cube " << picked;
        return Optional<Coords>(picked);
    }

    return Optional<Coords>();
}

void Cube::rotate(Axis axis, int coord, float angle) {
    CHECK(coord >= -1 && coord <= 1, "Coord mus be -1, 0 or 1");

    switch(axis) {
    case Axis::X:
        rotateX(coord, angle);
        break;

    case Axis::Y:
        rotateY(coord, angle);
        break;

    case Axis::Z:
        rotateZ(coord, angle);
    }
}

// FIXME: code duplication
void Cube::rotateX(int x, float angle) {
    static glm::vec3 rotationAxis(1, 0, 0);

    for (int y : {-1, 0, 1}) {
        for (int z : {-1, 0, 1}) {
            int id = getID(x, y, z);
            auto &modelRotation = rotations.at(id);
            auto newRotation = glm::angleAxis(angle, rotationAxis);
            modelRotation = newRotation * modelRotation;
        }
    }
}

void Cube::rotateY(int y, float angle) {
    static glm::vec3 rotationAxis(0, 1, 0);

    for (int x : {-1, 0, 1}) {
        for (int z : {-1, 0, 1}) {
            int id = getID(x, y, z);
            auto &modelRotation = rotations.at(id);
            auto newRotation = glm::angleAxis(angle, rotationAxis);
            modelRotation = newRotation * modelRotation;
        }
    }
}

void Cube::rotateZ(int z, float angle) {
    static glm::vec3 rotationAxis(0, 0, 1);

    for (int x : {-1, 0, 1}) {
        for (int y : {-1, 0, 1}) {
            int id = getID(x, y, z);
            auto &modelRotation = rotations.at(id);
            auto newRotation = glm::angleAxis(angle, rotationAxis);
            modelRotation = newRotation * modelRotation;
        }
    }
}

template <int N>
static bool compareCoords(const Coords &c1,
                          const Coords &c2,
                          const Coords &c3) {
    return std::get<N>(c1) == std::get<N>(c2) &&
        std::get<N>(c2) == std::get<N>(c3) &&
        std::get<N>(c1) == std::get<N>(c3);
}

Optional<pair<Axis, int>> findCommonAxis(const Coords &c1,
                                         const Coords &c2,
                                         const Coords &c3) {
    if (c1 == c2 || c2 == c3 || c1 == c3) {
        return Optional<std::pair<Axis, int>>();
    }

    bool commonX = compareCoords<0>(c1, c2, c3);
    bool commonY = compareCoords<1>(c1, c2, c3);
    bool commonZ = compareCoords<2>(c1, c2, c3);

    if (commonX && !commonY && !commonZ) {
        return Optional<pair<Axis, int>>(Axis::X, std::get<0>(c1));
    }
    if (!commonX && commonY && !commonZ) {
        return Optional<pair<Axis, int>>(Axis::Y, std::get<1>(c1));
    }
    if (!commonX && !commonY && commonZ) {
        return Optional<pair<Axis, int>>(Axis::Z, std::get<2>(c1));
    }
    return Optional<std::pair<Axis, int>>();
}

}

