#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include "util/io/stream_matcher.hpp"
#include "logging/logging.hpp"
#include "gl/model/obj_model_loader.hpp"

using namespace std;
using namespace glm;
using namespace io;

namespace gl {
namespace model {

static bool startsWith(const string &str, const string &pattern) {
    return strncmp(str.c_str(), pattern.c_str(), pattern.length()) == 0;
}

unique_ptr<Model> ObjModelLoader::fromFile(const char *filename) {
    LOG(INFO) << "Loading OBJ model: " << filename;

    vector<unsigned> vertexIndices;
    vector<unsigned> normalIndices;
    vector<unsigned> uvIndices;
    vector<vec3> verticesTemp;
    vector<vec3> normalsTemp;
    vector<vec2> uvsTemp;

    ifstream file(filename);
    if (!file) {
        LOG(ERROR) << "Can't open file: " << filename;
        return nullptr;
    }

    while (!file.eof()) {
        // get line
        string lineStr;
        getline(file, lineStr);
        if (file.eof()) {
            break;
        }
        istringstream line(lineStr);

        if (startsWith(lineStr, "v ")) {
            // parse vertex
            vec3 vertex;
            line >> match("v") >> vertex.x >> vertex.y >> vertex.z;
            verticesTemp.push_back(vertex);
        } else if (startsWith(lineStr, "vt")) {
            // parse texture coords
            vec2 uv;
            line >> match("vt") >> uv.x >> uv.y;
            uvsTemp.push_back(uv);
        } else if (startsWith(lineStr, "vn")) {
            // parse normal
            vec3 normal;
            line >> match("vn") >> normal.x >> normal.y >> normal.z;
            normalsTemp.push_back(normal);
        } else if (startsWith(lineStr, "f")) {
            // parse face
            unsigned vertexIndex[3];
            unsigned uvIndex[3];
            unsigned normalIndex[3];
            line >> match("f")
                >> vertexIndex[0] >> match("/") >> uvIndex[0] >> match("/") >> normalIndex[0]
                >> vertexIndex[1] >> match("/") >> uvIndex[1] >> match("/") >> normalIndex[1]
                >> vertexIndex[2] >> match("/") >> uvIndex[2] >> match("/") >> normalIndex[2];

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    file.close();

    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> uvs;
    for (auto &vertexIndex : vertexIndices) {
        vertices.push_back(verticesTemp[vertexIndex - 1]);
    }
    for (auto &uvIndex : uvIndices) {
        uvs.push_back(uvsTemp[uvIndex - 1]);
    }
    for (auto &normalIndex : normalIndices) {
        normals.push_back(normalsTemp[normalIndex - 1]);
    }

    auto *model = new Model{vertices, normals, uvs};
    return unique_ptr<Model>(model);
}

}
}

