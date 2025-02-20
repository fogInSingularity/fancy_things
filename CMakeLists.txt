cmake_minimum_required(VERSION 3.21) # current lts version

project(fancy_things)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
    src//main.cpp
    src/filter/filters.cpp
    src/fcy_ui/fancy_things.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include/
)

# set(CMAKE_CXX_COMPILER gcc)

set(ASAN_FLAGS "-fsanitize=address,leak,undefined")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fstack-protector-strong -fcheck-new -fstrict-overflow")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -ggdb ${ASAN_FLAGS}") # -D_FORTIFY_SOURCE=3
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -flto -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Og -g3 -ggdb ${ASAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O2 -march=native -flto -DNDEBUG")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# to output colors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always") # For all targets
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "sfml-graphics" "sfml-window" "sfml-system")

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
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
endif()

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.16.0
    OPTIONS "INSTALL_GTEST OFF"
)

enable_testing()
add_subdirectory(tests)
