cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("fancy_things")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
    src/source/main.cpp
    src/source/filters.cpp
    src/source/fancy_things.cpp
)

add_executable(fancy_things ${SRCS})

target_include_directories(fancy_things
    PUBLIC
        src/include/
        lib_cpp/include/
)

# set(CMAKE_CXX_COMPILER gcc)

# set(ASAN_FLAGS "-fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fstack-protector-strong -fcheck-new -fstrict-overflow")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -ggdb ${ASAN_FLAGS}") # -D_FORTIFY_SOURCE=3
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -flto -g3 -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Og -g3 -ggdb ${ASAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O2 -march=native -flto -g3 -DNDEBUG")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# to output colors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always") # For all targets
endif()

target_link_libraries(fancy_things PUBLIC "sfml-graphics" "sfml-window" "sfml-system")

# use cpm package manager
include(cmake/CPM.cmake)

message(STATUS "try to add spdlog:")

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    # GIT_TAG v2.x
    GIT_TAG v1.15.0
)

if (spdlog_ADDED)
    # message(STATUS "spdlog src dir: ${spdlog_SOURCE_DIR}")
    target_link_libraries(fancy_things PRIVATE spdlog)
    target_include_directories(fancy_things PUBLIC "${spdlog_SOURCE_DIR}/include/")
endif()

target_link_libraries(fancy_things PUBLIC "bfd" "dl")

if(USE_BACKWARD_LIB)
    # install libdw libbfd libdwarf
    message(STATUS "try to add backward:")

    CPMAddPackage(
        NAME backward-cpp
        GITHUB_REPOSITORY bombela/backward-cpp
        GIT_TAG v1.6 # Or latest version
    )

    if(backward-cpp_ADDED) 
        target_compile_definitions(fancy_things PRIVATE USE_BACKWARD_LIB)
        target_compile_definitions(fancy_things PRIVATE BACKWARD_HAS_BFD=1)
        # target_link_libraries(fancy_things PRIVATE backward-cpp)
        target_include_directories(fancy_things PUBLIC "${backward-cpp_SOURCE_DIR}")
    endif()
endif()
