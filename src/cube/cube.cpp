#include <stdexcept>
#include <list>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "logging/logging.hpp"
#include "gl/shader/shader.hpp"
#include "cube/cube.hpp"

using namespace gl::mesh;
using namespace gl::shader;
using namespace gl::texture;

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
    for (int i = 0; i < 3 * 3 * 3; ++i) {
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

std::pair<bool, Coords> Cube::testRayIntersection(const gl::picking::Ray &ray) {
    // same bounding box for all cubes
    static gl::picking::AABB aabb(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));

    // a list of all matched pairs: distance, coords of the cube
    std::list<std::pair<float, Coords>> matched;

    for (int i = 0; i < 3 * 3 * 3; ++i) {
        // FIXME: actual position, rotation included
        auto &model = modelMatrices.at(i);
        auto result = gl::picking::testRayOBBIntersection(ray, aabb, model);

        if (result.first) {
            auto coords = getCoords(i);
            matched.push_back(std::make_pair(result.second, coords));
            LOG(DEBUG) << "Matched cube " << coords << ", distance: " << result.second;
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
        return {true, picked};
    }

    return {false, Coords()};
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

}

