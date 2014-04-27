#ifndef MODEL_FACTORY_HPP_U2FC6MIV
#define MODEL_FACTORY_HPP_U2FC6MIV

#include <memory>

#include "model.hpp"

namespace gl {

class ModelFactory {
public:
    virtual ~ModelFactory() = default;

    virtual std::shared_ptr<Model> fromFile(const char *filename) = 0;
    virtual std::shared_ptr<Model> fromContents(const char *contents) = 0;
};

}

#endif /* end of include guard: MODEL_FACTORY_HPP_U2FC6MIV */

