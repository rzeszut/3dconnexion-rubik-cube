#ifndef OPTIONAL_HPP_AZHEIZP6
#define OPTIONAL_HPP_AZHEIZP6

#include <utility>
#include <stdexcept>

#include "util/preconditions.hpp"

namespace optional {

template <typename T>
class Optional {
private:
    bool present;
    T value;

public:
    Optional();
    Optional(const T &value);
    Optional(T &&value);
    Optional(const Optional &other);
    Optional(Optional &&other);

    template <typename... Args>
    Optional(Args&&... args);

    Optional &operator =(const Optional &other);
    Optional &operator =(Optional &&other);

    void reset();

    operator bool() const;
    bool isPresent() const;

    T &get();
    const T &get() const;

    T *operator ->();
    const T *operator ->() const;
    T &operator *();
    const T &operator *() const;
};

// ------------------ //
// inline definitions //
// ------------------ //

template <typename T>
Optional<T>::Optional() :
    present(false)
{}

template <typename T>
inline Optional<T>::Optional(const T &v) :
    present(true),
    value(v)
{}

template <typename T>
inline Optional<T>::Optional(T &&v) :
    present(true),
    value(std::move(v))
{}

template <typename T>
inline Optional<T>::Optional(const Optional &other) :
    present(other.present)
{
    if (present) {
        new (&value) T(other.value);
    }
}

template <typename T>
inline Optional<T>::Optional(Optional &&other) :
    present(other.present)
{
    if (present) {
        new (&value) T(std::move(other.value));
        other.present = false;
    }
}

template <typename T>
template <typename... Args>
Optional<T>::Optional(Args&&... args) :
    present(true),
    value(std::forward<Args>(args)...)
{}


template <typename T>
inline Optional<T> &Optional<T>::operator =(const Optional &other) {
    present = other.present;
    if (present) {
        new (&value) T(other.value);
    } else {
        value.~T();
    }

    return *this;
}

template <typename T>
inline Optional<T> &Optional<T>::operator =(Optional &&other) {
    present = other.present;
    if (present) {
        new (&value) T(std::move(other.value));
    } else {
        value.~T();
    }

    return *this;
}

template <typename T>
inline void Optional<T>::reset() {
    if (present) {
        present = false;
        value.~T();
    }
}

template <typename T>
inline Optional<T>::operator bool() const {
    return present;
}

template <typename T>
inline bool Optional<T>::isPresent() const {
    return present;
}

template <typename T>
inline T &Optional<T>::get() {
    CHECK(present, "Error: referencing empty optional.");
    return value;
}

template <typename T>
inline const T &Optional<T>::get() const {
    CHECK(present, "Error: referencing empty optional.");
    return value;
}

template <typename T>
inline T *Optional<T>::operator ->() {
    CHECK(present, "Error: referencing empty optional.");
    return &value;
}

template <typename T>
inline const T *Optional<T>::operator ->() const {
    CHECK(present, "Error: referencing empty optional.");
    return &value;
}

template <typename T>
inline T &Optional<T>::operator *() {
    CHECK(present, "Error: referencing empty optional.");
    return value;
}

template <typename T>
inline const T &Optional<T>::operator *() const {
    CHECK(present, "Error: referencing empty optional.");
    return value;
}

}

#endif /* end of include guard: OPTIONAL_HPP_AZHEIZP6 */

