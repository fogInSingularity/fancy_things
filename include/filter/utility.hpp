#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <cstddef>

namespace ftr {

#ifndef TO_STR
#define TO_STR(...) #__VA_ARGS__
#endif // TO_STR

class Size {
  public:
    size_t w = 0;
    size_t h = 0;
  public:
    Size(size_t width, size_t height) noexcept : w{width}, h{height} {}
};

} // namespace ftr

#endif // UTILITY_HPP_
