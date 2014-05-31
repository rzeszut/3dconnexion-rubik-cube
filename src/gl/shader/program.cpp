#include "logging/logging.hpp"
#include "gl/error.hpp"
#include "gl/shader/loader.hpp"
#include "gl/shader/program.hpp"
#include "gl/shader/shader.hpp"

using namespace std;

namespace gl {
namespace shader {

unique_ptr<Program> Program::fromShaders(initializer_list<shared_ptr<Shader>> shaders) {
    vector<GLuint> shaderIDs;
    for (auto &it : shaders) {
        if (!it) {
            return nullptr;
        }
        shaderIDs.push_back(it->getID());
    }

    GLuint program = compileProgram(shaderIDs);
    if (program == NULL_PROGRAM) {
        return nullptr;
    } else {
        return unique_ptr<Program>(new Program(program));
    }
}

}
}

