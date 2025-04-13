#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <cstdint>
#include <cstddef>
#include <type_traits>

#if defined (__linux__)
#include <unistd.h>
#endif // __linux__

namespace hlp {

template <typename EnumT, typename T = std::underlying_type_t<EnumT> >
constexpr inline EnumT ToEnum(T value) {
    return static_cast<EnumT>(value);
}

template <typename EnumT, typename T = std::underlying_type_t<EnumT> >
constexpr inline T FromEnum(EnumT enum_value) {
    return static_cast<T>(enum_value);
}

inline int64_t GetPid() {
#if defined (__linux__)
    return static_cast<int64_t>(getpid());
#else 
    return 0;
#endif // __linux__
}

#if defined (__cpp_lib_hardware_interference_size)
constexpr const size_t kCacheLineSize = std::hardware_destructive_interference_size;
#else 
constexpr const size_t kCacheLineSize = 64;
#endif // __cpp_lib_hardware_interference_size

} // namespace hlp
 
#endif // COMMON_HPP_

