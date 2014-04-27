#ifndef OBJ_MODEL_FACTORY_HPP_1ZZOFY7V
#define OBJ_MODEL_FACTORY_HPP_1ZZOFY7V

#include "model_factory.hpp"

namespace gl {

class ObjModelFactory : public ModelFactory {
private:
    ObjModelFactory() = default;

public:
    ~ObjModelFactory() = default;

    ObjModelFactory(const ObjModelFactory &) = delete;
    void operator=(const ObjModelFactory &) = delete;

    static ObjModelFactory &getInstance();

    std::shared_ptr<Model> fromFile(const char *filename) override;
    std::shared_ptr<Model> fromContents(const char *contents) override;
};

// ------------------ //
// inline definitions //
// ------------------ //
inline ObjModelFactory &ObjModelFactory::getInstance() {
    static ObjModelFactory f;
    return f;
}

}

#endif /* end of include guard: OBJ_MODEL_FACTORY_HPP_1ZZOFY7V */

