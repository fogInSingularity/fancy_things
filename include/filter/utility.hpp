#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <cstddef>
#include <type_traits>

namespace ftr {

#ifndef TO_STR
#define TO_STR(...) #__VA_ARGS__
#endif // TO_STR

template <typename T, typename = std::is_arithmetic<T>> 
class SizeT {
  public:
    T w = 0;
    T h = 0;
  public:
    SizeT(T width, T height) noexcept : w{width}, h{height} {}
    template <typename U>
    explicit SizeT(SizeT<U> size_u) : w{static_cast<T>(size_u.w)}, h{static_cast<T>(size_u.h)} {}
};

using Size = SizeT<size_t>;

} // namespace ftr

#endif // UTILITY_HPP_
