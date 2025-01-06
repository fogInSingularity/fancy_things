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
    lib_cpp/source/logging.c
)

add_executable(fancy_things ${SRCS})

target_include_directories(fancy_things
    PUBLIC
        src/include/
        lib_cpp/include/
)

target_link_libraries(fancy_things "sfml-graphics" "sfml-window" "sfml-system")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# to output colors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always") # For all targets
endif()
