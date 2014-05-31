#include <stdexcept>

#include "glm/gtc/matrix_transform.hpp"

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
                glm::mat4 &model = getModelMatrix(i, j, k);
                model = glm::translate(model, glm::vec3(2 * i, 2 * j, 2 * k));
            }
        }
    }
}

glm::mat4 &Cube::getModelMatrix(int i, int j, int k) {
    return modelMatrices.at(3 * 3 * (i + 1) + 3 * (j + 1) + (k + 1));
}

void Cube::render(const glm::mat4 &view, const glm::mat4 &projection) {
    for (const auto &model : modelMatrices) {
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

}

