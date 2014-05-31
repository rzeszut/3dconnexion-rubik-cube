#include <cstring>
#include <map>

#include "gl/mesh/index.hpp"

using namespace std;
using namespace glm;

namespace gl {
namespace mesh {

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;

    bool operator< (const Vertex &other) const {
        return memcmp(this, &other, sizeof(Vertex)) > 0;
    }
};

void indexMesh(const vector<vec3> &verticesIn,
               const vector<vec3> &normalsIn,
               const vector<vec2> &uvsIn,
               vector<vec3> &verticesOut,
               vector<vec3> &normalsOut,
               vector<vec2> &uvsOut,
               vector<unsigned> &elementsOut
              ) {

    unsigned index = 0;
    map<Vertex, unsigned> indexMap;

    const int size = verticesIn.size();
    for (int i = 0; i < size; ++i) {
        Vertex v = {verticesIn[i], normalsIn[i], uvsIn[i]};

        auto it = indexMap.find(v);
        if (it == indexMap.end()) {
            // new vertex
            indexMap[v] = index;
            elementsOut.push_back(index);
            verticesOut.push_back(v.position);
            normalsOut.push_back(v.normal);
            uvsOut.push_back(v.uv);
            ++index;
        } else {
            // already indexed
            elementsOut.push_back(it->second);
        }
    }
}

}
}

