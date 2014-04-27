#ifndef STREAM_MATCHER_HPP_UECGYA2D
#define STREAM_MATCHER_HPP_UECGYA2D

#include <string>
#include <istream>

namespace io {

template <typename Char,
         typename Traits = std::char_traits<Char>,
         typename Allocator = std::allocator<Char>>
class StreamMatcher {
private:
    std::basic_string<Char, Traits, Allocator> token;

public:
    StreamMatcher(const std::basic_string<Char, Traits, Allocator> &t);

    const std::basic_string<Char, Traits, Allocator> &getToken() const;
};

template <typename Char>
StreamMatcher<Char> match(const std::basic_string<Char> &string);

template<typename Char, typename Traits, typename Allocator>
std::basic_istream<Char, Traits> &operator>>(std::basic_istream<Char, Traits> &in,
                                             const StreamMatcher<Char, Traits, Allocator> &matcher);

// ------------------ //
// inline definitions //
// ------------------ //
template<typename Char, typename Traits, typename Allocator>
inline StreamMatcher<Char, Traits, Allocator>::StreamMatcher(const std::basic_string<Char, Traits, Allocator> &t) :
    token(t)
{}

template<typename Char, typename Traits, typename Allocator>
inline const std::basic_string<Char, Traits, Allocator> &StreamMatcher<Char, Traits, Allocator>::getToken() const {
    return token;
}

template <typename Char>
inline StreamMatcher<Char> match(const Char *string) {
    return StreamMatcher<Char>(std::basic_string<Char>(string));
}

template <typename Char>
inline StreamMatcher<Char> match(const std::basic_string<Char> &string) {
    return StreamMatcher<Char>(string);
}

template<typename Char, typename Traits, typename Allocator>
std::basic_istream<Char, Traits> &operator>>(std::basic_istream<Char, Traits> &in,
                                             const StreamMatcher<Char, Traits, Allocator> &matcher) {
    auto it = matcher.getToken().begin();
    auto end = matcher.getToken().end();
    Char c;

    while (it != end && in.get(c)) {
        if (*it == c) {
            ++it;
        } else {
            in.clear(std::basic_ios<Char, Traits>::failbit);
            break;
        }
    }

    return in;
}

}

#endif /* end of include guard: STREAM_MATCHER_HPP_UECGYA2D */

