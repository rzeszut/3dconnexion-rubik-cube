#ifndef PRECONDITIONS_HPP_Y1NSGXPH
#define PRECONDITIONS_HPP_Y1NSGXPH

#include <exception>
#include <string>
#include <sstream>

#include "util.hpp"

namespace preconditions {

#define CHECK(cond, message) if (!(cond)) throw preconditions::PreconditionFailedException(__FILENAME__, __LINE__, __func__, (message));

#define CHECK_NOT_NULL(obj, message) if ((obj) == nullptr) throw preconditions::PreconditionFailedException(__FILENAME__, __LINE__, __func__, (message));

class PreconditionFailedException final : public std::exception {
private:
    std::string filename;
    int line;
    std::string function;
    std::string message;

public:
    PreconditionFailedException(std::string filename,
                                int line,
                                std::string function,
                                std::string message) noexcept;

    const char *what() const noexcept;
};

// ------------------ //
// inline definitions //
// ------------------ //
inline PreconditionFailedException::PreconditionFailedException(std::string f,
                                int l,
                                std::string fn,
                                std::string m) noexcept :
    filename(f), line(l), function(fn), message(m)
{}

inline const char *PreconditionFailedException::what() const noexcept {
    std::ostringstream msg;
    msg << "Precondition failed: " << filename << ":" << line;
    msg << " at " << function << "() -- " << message;
    return msg.str().c_str();
}

}

#endif /* end of include guard: PRECONDITIONS_HPP_Y1NSGXPH */

