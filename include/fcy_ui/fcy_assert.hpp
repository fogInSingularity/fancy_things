#ifndef FCY_ASSERT_HPP_
#define FCY_ASSERT_HPP_

#include <iostream>
#include <string>
#include <cstdlib>

#include "spdlog/spdlog.h"

#if defined (NDEBUG)
#define fcy_assert(expr) (void)0
#else // NDEBUG
#define fcy_assert(expr) fcy_assert_(expr, #expr, __FILE__, __LINE__, __func__)
#endif // NDEBUG

inline void fcy_assert_(bool expr, const char* expr_str, const char* src_file, int src_line, const char* src_func) {
    if (expr) { return ; }

    std::cerr << "fcy_assert failed: " << std::string(expr_str) << "\n";
    std::cerr << "location: " 
              << src_file
              << ":" 
              << src_line 
              << " in function: " 
              << src_func
              << "\n";

    spdlog::critical("fcy_assert failed: {} ", expr_str);
    spdlog::critical("location: {}:{}", src_file, src_line);
    spdlog::critical("function: {}", src_func);

    std::abort();
}

#endif // FCY_ASSERT_HPP_
