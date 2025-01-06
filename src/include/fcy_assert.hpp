#ifndef FCY_ASSERT_HPP_
#define FCY_ASSERT_HPP_

#include <iostream>
#include <string>
#include <cstdlib>

// #undef USE_BACKWARD_LIB
#if defined (USE_BACKWARD_LIB)
#include "backward.hpp"
#elif defined (__linux__)
#include <execinfo.h>
#endif // __linux__

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

    spdlog::critical("fcy_assert failed: {} ", expr_str);
    spdlog::critical("location: {}:{}", src_file, src_line);
    spdlog::critical("function: {}", src_func);

#if defined (USE_BACKWARD_LIB)
    backward::StackTrace stack_trace;
    stack_trace.load_here();

    backward::TraceResolver trace_resolver;
    trace_resolver.load_stacktrace(stack_trace);

    spdlog::critical("stack trace:");
    for (int i = 2; i < stack_trace.size(); i++) {
        backward::ResolvedTrace trace = trace_resolver.resolve(stack_trace[i]);
        spdlog::critical("# {}: \n\t{}:{}\n\t{}", 
                         i-2, trace.source.filename, trace.source.line, trace.source.function);
    }
#elif defined (__linux__)
    const size_t kMaxTraceDepth = 32;
    void* stack_trace_addreses[kMaxTraceDepth] = {0};

    int trace_depth = backtrace(stack_trace_addreses, kMaxTraceDepth);
    char** trace = backtrace_symbols(stack_trace_addreses, trace_depth);

    spdlog::critical("stack trace:");
    for (int i = 0; i < trace_depth; i++) {
        spdlog::critical("{}", trace[i]);
    }
#endif // __linux__

    std::abort();
}

#endif // FCY_ASSERT_HPP_
