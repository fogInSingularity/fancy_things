cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("fancy_things")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SRCS
    src/source/main.cpp
    src/source/pixel.cpp
    src/source/filters.cpp
    src/source/fancy_things.cpp
)

add_executable(fancy_things ${SRCS})

target_include_directories(fancy_things
    PUBLIC
        src/include/
        lib_cpp/include/
)

target_link_libraries(fancy_things PUBLIC "sfml-graphics" "sfml-window" "sfml-system")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# to output colors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always") # For all targets
endif()

# use cpm package manager
include(cmake/CPM.cmake)

message(STATUS "try to add spdlog:")

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.15.0
)

if (spdlog_ADDED)
    # message(STATUS "spdlog src dir: ${spdlog_SOURCE_DIR}")
    target_link_libraries(fancy_things PRIVATE spdlog)
    target_include_directories(fancy_things PUBLIC "${spdlog_SOURCE_DIR}/include/")
endif()
