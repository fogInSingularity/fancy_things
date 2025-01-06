#ifndef FCY_ASSERT_HPP_
#define FCY_ASSERT_HPP_

#include <iostream>
#include <string>
#include <cstdlib>

#include "spdlog/spdlog.h"

#if defined (NDEBUG)
#define (void)0
#else // NDEBUG
#define fcy_assert(...) fcy_assert_(__VA_ARGS__, #__VA_ARGS__, __FILE__, __LINE__, __func__)
#endif // NDEBUG

inline void fcy_assert_(bool expr, const char* expr_str, const char* src_file, int src_line, const char* src_func) {
    if (expr) { return ; }

    std::cerr << "fcy_assert failed: " << std::string(expr_str) << "\n";
    std::cerr << "location: " 
              << std::string(src_file) 
              << ":" 
              << src_line 
              << " in function: " 
              << std::string(src_func) 
              << "\n";

    spdlog::critical("fcy_assert failed: {} in {}:{} in function: {}", 
                     expr_str, src_file, src_line, src_func);

    std::abort();
}

#endif // FCY_ASSERT_HPP_
