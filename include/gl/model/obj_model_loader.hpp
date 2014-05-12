#ifndef OBJ_MODEL_LOADER_HPP_WOIGULVC
#define OBJ_MODEL_LOADER_HPP_WOIGULVC

#include <memory>

#include "model.hpp"

namespace gl {
namespace model {

class ObjModelLoader {
public:
    static std::unique_ptr<Model> fromFile(const char *filename);
};

}
}

#endif /* end of include guard: OBJ_MODEL_LOADER_HPP_WOIGULVC */

